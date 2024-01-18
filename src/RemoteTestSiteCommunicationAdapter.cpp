#include "RemoteTestSiteCommunicationAdapter.hpp"
#include "IRemoteTestSiteController.hpp"
#include <pb_encode.h>
#include <pb_decode.h>
#include <pb_common.h>

RemoteTestSiteCommunicationAdapter::RemoteTestSiteCommunicationAdapter(IRemoteTestSiteController *const controller)
{
    this->controller = controller;
    // All ids must initialize with 0 and be given an id when joining network
    this->id = 0;
}

pb_ostream_t RemoteTestSiteCommunicationAdapter::CreateOutputStream(uint8_t *const message_buffer)
{
    return pb_ostream_from_buffer(message_buffer, sizeof(message_buffer));
}

bool RemoteTestSiteCommunicationAdapter::EncodeBaseMessage(pb_ostream_t *const stream, const uint64_t destination_id)
{
    /* Create base message. */
    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;
    message.sender_id = this->id;
    message.has_sender_id = true;

    // EncodeJoin(&stream, 2);
    // EncodeMeasurement(&stream, RemoteTestSite_MeasurementType_MEASUREMENT_TYPE_TEMPERATURE, 17.1);
    // RemoteTestSite_Timestamp timestamp = RemoteTestSite_Timestamp_init_zero;
    // timestamp.seconds = 123;
    // timestamp.nanos = 456;
    // EncodeUpdate(&stream, RemoteTestSite_MeasurementType_MEASUREMENT_TYPE_TEMPERATURE, timestamp);
    // EncodePing(&stream);
    // EncodeResponse(&stream, RemoteTestSite_ErrorCode_ERROR_CODE_OK);

    /* Encode the message. */
    return pb_encode(stream, RemoteTestSite_Message_fields, &message);
}

bool RemoteTestSiteCommunicationAdapter::HandleMessage(pb_istream_t *stream)
{
    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;

    /* Now we are ready to decode the message. */
    bool status = pb_decode(stream, RemoteTestSite_Message_fields, &message);

    /* Check for errors... */
    if (!status)
    {
        return false;
        // Serial.print("Decoding failed: ");
        // Serial.println(PB_GET_ERROR(&stream));
    }

    /* Check if message is meant for this node */
    if (message.has_target_id)
    {
        if (this->id != message.target_id)
        {
            /* Message not meant for this node. */
            return true;
        }
    }

    switch (message.which_function_info)
    {
    case RemoteTestSite_Message_join_tag:
        if (message.function_info.join.has_new_id)
        {
            return this->controller->Join(message.function_info.join.new_id);
            // Serial.print("Join message new id: ");
            // Serial.println((int)message.function_info.join.new_id);
        }

        break;
    case RemoteTestSite_Message_measurement_tag:
        if (message.function_info.measurement.has_type && message.function_info.measurement.has_value)
        {
            return this->controller->Measurement(message.function_info.measurement.type, message.function_info.measurement.value);
            // Serial.print("Measurement of type: ");
            // Serial.println((int)message.function_info.measurement.type);
            // Serial.print("Value: ");
            // Serial.println((float)message.function_info.measurement.value);
        }

        break;
    case RemoteTestSite_Message_update_tag:
        // message.function_info.update.which_UpdateType;
        // if ()
        // {
        //     // TODO: make sitch for update types
        //     return this->controller->UpdateFrequency(message.function_info.update.type, message.function_info.update.frequency);
        //     // Serial.print("Update of type: ");
        //     // Serial.println((int)message.function_info.update.type);
        //     // Serial.print("Frequency in second:nanoseconds : ");
        //     // Serial.print((int)message.function_info.update.frequency.seconds);
        //     // Serial.print(':');
        //     // Serial.println((int)message.function_info.update.frequency.nanos);
        // }

        break;
    case RemoteTestSite_Message_ping_tag:
        // Serial.println("Ping received.");
        return this->controller->Ping(message.sender_id);

        break;
    case RemoteTestSite_Message_response_tag:
        if (message.function_info.response.has_response_code)
        {
            return controller->Response(message.function_info.response.response_code);
            // if ((int)message.function_info.response.error_code == RemoteTestSite_ErrorCode::RemoteTestSite_ErrorCode_ERROR_CODE_OK)
            // {
            //     Serial.println("Response OK");
            // }
            // else
            // {
            //     Serial.print("Error code: ");
            //     Serial.println((int)message.function_info.response.error_code);
            // }
        }

        break;

    default:
        return this->controller->UnknownMessageType(message.which_function_info);
        // Serial.print("Function unknown: ");
        // Serial.println((int)message.which_function_info);
        break;
    }
    return false;
}

void RemoteTestSiteCommunicationAdapter::SetId(uint64_t new_id)
{
    this->id = new_id;
}

bool RemoteTestSiteCommunicationAdapter::EncodeJoin(pb_ostream_t *const stream, const uint64_t destination_id, uint64_t new_id)
{
    EncodeBaseMessage(stream, destination_id);

    /* Create join message type. */
    RemoteTestSite_Join join = RemoteTestSite_Join_init_zero;
    join.new_id = new_id;
    join.has_new_id = true;

    return EncodeUnionmessage(stream, RemoteTestSite_Join_fields, &join);
}

bool RemoteTestSiteCommunicationAdapter::EncodeMeasurement(pb_ostream_t *const stream, const uint64_t destination_id, const RemoteTestSite_MeasurementType type, const float value)
{
    EncodeBaseMessage(stream, destination_id);

    /* Create measurement message type. */
    RemoteTestSite_Measurement measurement = RemoteTestSite_Measurement_init_zero;
    measurement.type = type;
    measurement.has_type = true;
    measurement.value = value;
    measurement.has_value = true;

    return EncodeUnionmessage(stream, RemoteTestSite_Measurement_fields, &measurement);
}

bool RemoteTestSiteCommunicationAdapter::EncodeUpdate(pb_ostream_t *const stream, const uint64_t destination_id, const RemoteTestSite_MeasurementType type, const RemoteTestSite_Timestamp frequency)
{
    EncodeBaseMessage(stream, destination_id);

    /* Create update message type. */
    RemoteTestSite_Update update = RemoteTestSite_Update_init_zero;
    update.type = type;
    update.has_type = true;
    update.frequency = frequency;
    update.has_frequency = true;

    return EncodeUnionmessage(stream, RemoteTestSite_Update_fields, &update);
}

bool RemoteTestSiteCommunicationAdapter::EncodePing(pb_ostream_t *const stream, const uint64_t destination_id)
{
    EncodeBaseMessage(stream, destination_id);

    /* Create ping message type. */
    RemoteTestSite_Ping ping = RemoteTestSite_Ping_init_zero;

    return EncodeUnionmessage(stream, RemoteTestSite_Ping_fields, &ping);
}

bool RemoteTestSiteCommunicationAdapter::EncodeResponse(pb_ostream_t *const stream, const uint64_t destination_id, const RemoteTestSite_ResponseCode code)
{
    EncodeBaseMessage(stream, destination_id);

    /* Create response message type. */
    RemoteTestSite_Response response = RemoteTestSite_Response_init_zero;
    response.response_code = code;
    response.has_response_code = true;

    return EncodeUnionmessage(stream, RemoteTestSite_Response_fields, &response);
}

bool RemoteTestSiteCommunicationAdapter::EncodeUnionmessage(pb_ostream_t *stream, const pb_msgdesc_t *messagetype, void *message)
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
                return false;

            return pb_encode_submessage(stream, messagetype, message);
        }
    } while (pb_field_iter_next(&iter));

    /* Didn't find the field for messagetype */
    return false;
}
