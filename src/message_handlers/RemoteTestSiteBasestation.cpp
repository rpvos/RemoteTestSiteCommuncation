#include "message_handlers/RemoteTestSiteBasestation.hpp"
#include <Arduino.h>

RemoteTestSiteBasestation::RemoteTestSiteBasestation(IConnectionHandler *const connection_handler)
{
    this->connection_handler = connection_handler;
}

void RemoteTestSiteBasestation::HandleMessage(const RemoteTestSite_Message &message)
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
    case RemoteTestSite_Message_syn_tag:
        Syn(message);
        break;
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

bool RemoteTestSiteBasestation::Syn(const RemoteTestSite_Message &message)
{
    RemoteTestSite_Message response = RemoteTestSite_Message_init_zero;
    response.which_function_info = RemoteTestSite_Message_syn_tag;
    response.has_acknowledge_number = true;
    response.acknowledge_number = message.sequence_number;
    this->connection_handler->Write(response);

    // TODO: improve performance to do other things while waiting
    // This can be done in other handle of message
    {
        while (!this->connection_handler->Available())
        {
            delay(100);
        }

        if (this->connection_handler->Available())
        {
            this->connection_handler->Read();
        }
    }

    return true;
}

bool RemoteTestSiteBasestation::Measurement(RemoteTestSite_MeasurementInfo type, int32_t value)
{
    Serial.print("Measurement of type: ");
    Serial.println((int)type);
    Serial.print("Value: ");
    Serial.println((float)value);
    return true;
}

bool RemoteTestSiteBasestation::UpdateFrequency(RemoteTestSite_MeasurementInfo type, RemoteTestSite_Timestamp frequency)
{

    Serial.print("Update of type: ");
    Serial.println((int)type);
    Serial.print("Frequency in second:nanoseconds : ");
    Serial.print((int)frequency.seconds);
    Serial.print(':');
    Serial.println((int)frequency.nanos);

    return true;
}

bool RemoteTestSiteBasestation::Response(RemoteTestSite_ResponseCode response_code)
{
    if (response_code == RemoteTestSite_ResponseCode::RemoteTestSite_ResponseCode_RESPONSE_CODE_SUCCES)
    {

        Serial.println("Response OK");
    }
    else
    {

        Serial.print("Error code: ");
        Serial.println((int)response_code);
    }
    return true;
}

bool RemoteTestSiteBasestation::UnknownMessageType(pb_size_t which_function_info)
{

    Serial.print("Function unknown: ");
    Serial.println((int)which_function_info);

    return true;
}
