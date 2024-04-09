#include "message_handlers/RemoteTestSiteSensorNode.hpp"

#include <Arduino.h>

void RemoteTestSiteSensorNode::HandleMessage(const RemoteTestSite_Message &message)
{
    /* Check if message is meant for this node */
    if (message.has_target_id)
    {
        // TODO: check for id in handle or in connection handler
        // if (this->id != message.target_id)
        // {
        //     /* Message not meant for this node. */
        //     return true;
        // }
    }

    switch (message.which_function_info)
    {
    case RemoteTestSite_Message_measurement_tag:
        if (message.function_info.measurement.has_info && message.function_info.measurement.has_value)
        {
            Measurement(message.function_info.measurement.info, message.function_info.measurement.value);
        }

        break;
    case RemoteTestSite_Message_update_tag:
        // TODO: make sitch for update types
        UpdateFrequency(message.function_info.update.UpdateType.update_frequency.info, message.function_info.update.UpdateType.update_frequency.frequency);
        break;
    case RemoteTestSite_Message_response_tag:
        if (message.function_info.response.has_response_code)
        {
            Response(message.function_info.response.response_code);
        }

        break;

    default:
        UnknownMessageType(message.which_function_info);
        break;
    }
}

bool RemoteTestSiteSensorNode::Measurement(RemoteTestSite_MeasurementInfo type, int32_t value)
{
#ifndef PIO_UNIT_TESTING
    Serial.print("Measurement of type: ");
    Serial.println((int)type);
    Serial.print("Value: ");
    Serial.println((float)value);
#endif
    return true;
}

bool RemoteTestSiteSensorNode::UpdateFrequency(RemoteTestSite_MeasurementInfo type, RemoteTestSite_Timestamp frequency)
{
#ifndef PIO_UNIT_TESTING
    Serial.print("Update of type: ");
    Serial.println((int)type);
    Serial.print("Frequency in second:nanoseconds : ");
    Serial.print((int)frequency.seconds);
    Serial.print(':');
    Serial.println((int)frequency.nanos);
#endif
    return true;
}

bool RemoteTestSiteSensorNode::Response(RemoteTestSite_ResponseCode response_code)
{
    if (response_code == RemoteTestSite_ResponseCode::RemoteTestSite_ResponseCode_RESPONSE_CODE_SUCCES)
    {
#ifndef PIO_UNIT_TESTING
        Serial.println("Response OK");
#endif
    }
    else
    {
#ifndef PIO_UNIT_TESTING
        Serial.print("Error code: ");
        Serial.println((int)response_code);
#endif
    }
    return true;
}

bool RemoteTestSiteSensorNode::UnknownMessageType(pb_size_t which_function_info)
{
#ifndef PIO_UNIT_TESTING
    Serial.print("Function unknown: ");
    Serial.println((int)which_function_info);
#endif
    return true;
}
