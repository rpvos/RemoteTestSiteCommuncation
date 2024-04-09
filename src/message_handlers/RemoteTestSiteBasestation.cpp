#include "message_handlers/RemoteTestSiteBasestation.hpp"
#include <Arduino.h>
#include <therm200_helper.hpp>
#include <vh400_helper.hpp>
#include <murata_soil_sensor_helper.h>

#ifndef kOperatingVoltage
#define kOperatingVoltage 3
#endif

#ifndef kAdcBits
#define kAdcBits 10
#endif

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
    case RemoteTestSite_Message_ack_tag:
        // Message was acknowledged
        // TODO: remove message from not acknowledged queue
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
    switch (type)
    {
    case RemoteTestSite_MeasurementInfo_MEASUREMENT_INFO_THERM200_TEMPERATURE:
        Serial.print(",THERM200,Temperature,");
        Serial.println(Therm200Helper::CalculateTemperature(value, kOperatingVoltage, kAdcBits));
        break;

    case RemoteTestSite_MeasurementInfo_MEASUREMENT_INFO_VH400_VWC:
        Serial.print(",VH400,VWC,");
        Serial.println(Vh400Helper::CalculateVwc(value, kOperatingVoltage, kAdcBits));
        break;

    case RemoteTestSite_MeasurementInfo_MEASUREMENT_INFO_MURATA_TEMPERATURE:
        Serial.print(",Murata soil sensor,Temperature,");
        Serial.println(MurataSoilSensorHelper::CalculateTemperature(value));
        break;
    case RemoteTestSite_MeasurementInfo_MEASUREMENT_INFO_MURATA_VWC:
        Serial.print(",Murata soil sensor,VWC,");
        Serial.println(MurataSoilSensorHelper::CalculateVWC(value));
        break;
    case RemoteTestSite_MeasurementInfo_MEASUREMENT_INFO_MURATA_EC_BULK:
        Serial.print(",Murata soil sensor,EC bulk,");
        Serial.println(MurataSoilSensorHelper::CalculateECBulk(value));
        break;
    case RemoteTestSite_MeasurementInfo_MEASUREMENT_INFO_MURATA_EC_PORE:
        Serial.print(",Murata soil sensor,EC pore,");
        Serial.println(MurataSoilSensorHelper::CalculateECPore(value));
        break;

    default:
        Serial.print(",Unknown type,");
        Serial.print((int)type);
        Serial.print(",");
        Serial.println((float)value);
        break;
    }
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

void RemoteTestSiteBasestation::SetConnectionHandler(IConnectionHandler *const connection_handler)
{
    this->connection_handler = connection_handler;
}