#include "proto_handlers/proto_helper.hpp"
#include <stdlib.h>

#include <pb_encode.h>
#include <pb_decode.h>
#include <pb_common.h>
#include <crc32.hpp>

ProtoHelper::ProtoHelper(uint64_t id)
{
    this->id = id;
    this->destination_id = 0;
    this->sequence_number = 0;
    this->acknowledge_number = 0;
}

ProtoHelper::~ProtoHelper()
{
}

uint64_t ProtoHelper::GetNewSequenceNumber()
{
    // Construct long long random number
    return random() << sizeof(long) | random();
}

pb_ostream_t ProtoHelper::CreateOutputStream(uint8_t *const message_buffer, const size_t message_buffer_size)
{
    return pb_ostream_from_buffer(message_buffer, message_buffer_size);
}

pb_istream_t ProtoHelper::CreateInputStream(const uint8_t *const message_buffer, const size_t message_length)
{
    return pb_istream_from_buffer(message_buffer, message_length);
}

bool ProtoHelper::EncodeBaseMessage(pb_ostream_t *const stream, RemoteTestSite_Message &message)
{
    message.sender_id = this->id;
    message.has_sender_id = true;
    message.target_id = this->destination_id;
    message.has_target_id = true;

    message.has_sequence_number = true;
    message.sequence_number = this->sequence_number++;

    const size_t buffer_size = 128;
    uint8_t buffer[buffer_size];
    bool crc = this->AddCrc(buffer, buffer_size, message);
    if (!crc)
    {
        // Crc was not created succesfully
        return 0;
    }

    /* Encode the message. */
    return pb_encode(stream, RemoteTestSite_Message_fields, &message);
}

bool ProtoHelper::AddCrc(uint8_t *buffer, const uint8_t buffer_size, RemoteTestSite_Message &message)
{
    message.has_checksum = true;
    message.checksum = CalculateCrc(message, buffer, buffer_size);
    return true;
}

bool ProtoHelper::EncodeMeasurement(pb_ostream_t *const stream, const RemoteTestSite_MeasurementType type, const float value)
{
    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;
    message.which_function_info = RemoteTestSite_Message_measurement_tag;
    EncodeBaseMessage(stream, message);

    /* Create measurement message type. */
    RemoteTestSite_Measurement measurement = RemoteTestSite_Measurement_init_zero;
    measurement.type = type;
    measurement.has_type = true;
    measurement.value = value;
    measurement.has_value = true;

    return EncodeUnionmessage(stream, RemoteTestSite_Measurement_fields, &measurement);
}

bool ProtoHelper::EncodeUpdate(pb_ostream_t *const stream, const RemoteTestSite_MeasurementType type, const RemoteTestSite_Timestamp frequency)
{
    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;
    message.which_function_info = RemoteTestSite_Message_update_tag;
    EncodeBaseMessage(stream, message);

    /* Create update message type. */
    RemoteTestSite_Update update = RemoteTestSite_Update_init_zero;
    RemoteTestSite_Update_UpdateFrequency update_frequency = RemoteTestSite_Update_UpdateFrequency_init_zero;
    update_frequency.type = type;
    update_frequency.has_type = true;
    update_frequency.frequency = frequency;
    update_frequency.has_frequency = true;
    EncodeUnionmessage(stream, RemoteTestSite_Update_UpdateFrequency_fields, &update_frequency);

    return EncodeUnionmessage(stream, RemoteTestSite_Update_fields, &update);
}

bool ProtoHelper::EncodeResponse(pb_ostream_t *const stream, const RemoteTestSite_ResponseCode code)
{
    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;
    message.which_function_info = RemoteTestSite_Message_response_tag;
    EncodeBaseMessage(stream, message);

    /* Create response message type. */
    RemoteTestSite_Response response = RemoteTestSite_Response_init_zero;
    response.response_code = code;
    response.has_response_code = true;

    return EncodeUnionmessage(stream, RemoteTestSite_Response_fields, &response);
}

bool ProtoHelper::EncodeUnionmessage(pb_ostream_t *stream, const pb_msgdesc_t *messagetype, void *message)
{

    pb_field_iter_t iter;

    if (!pb_field_iter_begin(&iter, RemoteTestSite_Message_fields, message))
        return false;

    do
    {
        if (iter.submsg_desc == messagetype)
        {
            /* This is our field, encode the message using it. */
            if (!pb_encode_tag_for_field(stream, &iter))
            {
                return false;
            }

            return pb_encode_submessage(stream, messagetype, message);
        }
    } while (pb_field_iter_next(&iter));

    /* Didn't find the field for messagetype */
    return false;
}

uint32_t ProtoHelper::CalculateCrc(RemoteTestSite_Message &message, uint8_t *buffer, size_t buffer_size)
{
    uint8_t cursor = 0;

    // TODO: check for buffer size
    memcpy(buffer + cursor, &message.sender_id, sizeof(message.sender_id));
    cursor += sizeof(message.sender_id);
    memcpy(buffer + cursor, &message.target_id, sizeof(message.target_id));
    cursor += sizeof(message.target_id);
    memcpy(buffer + cursor, &message.sequence_number, sizeof(message.sequence_number));
    cursor += sizeof(message.sequence_number);
    memcpy(buffer + cursor, &message.acknowledge_number, sizeof(message.acknowledge_number));
    cursor += sizeof(message.acknowledge_number);

    return Crc::Crc32B(buffer, cursor / sizeof(uint8_t));
}