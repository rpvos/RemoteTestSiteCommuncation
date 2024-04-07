#include "proto_handlers/proto_handler.hpp"

#include <pb_encode.h>
#include <pb_decode.h>
#include <stdlib.h>

ProtoHandler::ProtoHandler(ProtoHelper *const proto_helper, uint32_t random_seed)
{
    this->proto_helper = proto_helper;
    this->need_acknoledgement = false;

    srand(random_seed);
}

ProtoHandler::~ProtoHandler()
{
    // Maybe close connection if it is open but not decided yet
}

uint64_t ProtoHandler::GetSequenceNumber()
{
    return proto_helper->sequence_number;
}

bool ProtoHandler::SetId(uint64_t id)
{
    proto_helper->id = id;
    return true;
}

uint32_t ProtoHandler::GetCrc(RemoteTestSite_Message &message, uint8_t *buffer, size_t buffer_size)
{
    return proto_helper->CalculateCrc(message, buffer, buffer_size);
}

size_t ProtoHandler::EncodeMessage(OUT uint8_t *data, const size_t data_size, RemoteTestSite_Message message)
{
    if (this->need_acknoledgement)
    {
        message.has_acknowledge_number = true;
        message.acknowledge_number = proto_helper->acknowledge_number;
        this->need_acknoledgement = false;
    }

    pb_ostream_t output_stream = proto_helper->CreateOutputStream(data, data_size);

    /* Encode type of message based on function. */
    switch (message.which_function_info)
    {
    case RemoteTestSite_Message_syn_tag:
        // Syn needs new sequence number
        proto_helper->sequence_number = proto_helper->GetNewSequenceNumber();
        message.sequence_number = proto_helper->sequence_number++;
        break;
    case RemoteTestSite_Message_ack_tag:
        // Nothing has to be done
        break;
    case RemoteTestSite_Message_fin_tag:
        // Nothing has to be done
        break;
    case RemoteTestSite_Message_measurement_tag:
        proto_helper->EncodeMeasurement(&output_stream, message.function_info.measurement.info, message.function_info.measurement.value);
        break;
    case RemoteTestSite_Message_update_tag:
        proto_helper->EncodeUpdate(&output_stream, message.function_info.update.UpdateType.update_frequency.info, message.function_info.update.UpdateType.update_frequency.frequency);
        break;
    case RemoteTestSite_Message_response_tag:
        proto_helper->EncodeResponse(&output_stream, message.function_info.response.response_code);
        break;

    default:
        return 0;
        break;
    }
    if (!proto_helper->EncodeBaseMessage(&output_stream, message))
    {
        return 0;
    }
    return output_stream.bytes_written;
}

bool ProtoHandler::DecodeMessage(OUT RemoteTestSite_Message &message, const uint8_t *data, const size_t data_length)
{
    pb_istream_t input_stream = proto_helper->CreateInputStream(data, data_length);
    return pb_decode(&input_stream, RemoteTestSite_Message_fields, &message);
}
