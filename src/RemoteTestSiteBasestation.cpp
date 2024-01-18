#include "RemoteTestSiteBasestation.hpp"

#include <Arduino.h>

RemoteTestSiteBasestation::RemoteTestSiteBasestation() : communication_adapter(this)
{
}

bool RemoteTestSiteBasestation::Join(uint64_t new_id)
{
    Serial.print("Join message new id: ");
    Serial.println((int)new_id);
    return true;
}

bool RemoteTestSiteBasestation::Measurement(RemoteTestSite_MeasurementType type, float value)
{
    Serial.print("Measurement of type: ");
    Serial.println((int)type);
    Serial.print("Value: ");
    Serial.println((float)value);
    return true;
}

bool RemoteTestSiteBasestation::UpdateFrequency(RemoteTestSite_MeasurementType type, RemoteTestSite_Timestamp frequency)
{
    Serial.print("Update of type: ");
    Serial.println((int)type);
    Serial.print("Frequency in second:nanoseconds : ");
    Serial.print((int)frequency.seconds);
    Serial.print(':');
    Serial.println((int)frequency.nanos);
    return true;
}

bool RemoteTestSiteBasestation::Ping(uint64_t sender_id)
{
    Serial.print("Ping received from ");
    Serial.println((int)sender_id);
    return true;
}

bool RemoteTestSiteBasestation::Response(RemoteTestSite_ResponseCode response_code)
{
    if (response_code == RemoteTestSite_ResponseCode::RemoteTestSite_ResponseCode_RESPONSE_CODE_OK)
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
