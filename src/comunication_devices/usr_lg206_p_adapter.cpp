#include "comunication_devices/usr_lg206_p_adapter.hpp"

UsrLg206PAdapter::UsrLg206PAdapter()
{
    id = "";
}

UsrLg206PAdapter::~UsrLg206PAdapter()
{
}

bool UsrLg206PAdapter::Begin()
{
    if (lora.BeginAtMode() != LoRaErrorCode::kSucces)
    {
        Serial.println('1');
        return false;
    }

    LoRaErrorCode code = LoRaErrorCode::kNoResponse;
    for (size_t i = 0; i < 10 && code != LoRaErrorCode::kSucces; i++)
    {
        if (lora.SetWorkMode(LoRaSettings::WorkMode::kWorkModeTransparent) != LoRaErrorCode::kSucces)
        {
            Serial.println('2');
            break;
        }
        if (lora.SetAirRateLevel(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel268) != LoRaErrorCode::kSucces)
        {
            Serial.println('3');
            break;
        }
        if (lora.SetChannel(40) != LoRaErrorCode::kSucces)
        {
            Serial.println('4');
            break;
        }
        if (lora.SetDestinationAddress(1) != LoRaErrorCode::kSucces)
        {
            Serial.println('5');
            break;
        }
        if (lora.GetNodeId(this->id) != LoRaErrorCode::kSucces)
        {
            Serial.println('6');
            break;
        }
        if (lora.EndAtMode() != LoRaErrorCode::kSucces)
        {
            Serial.println('7');
            break;
        }
        code = LoRaErrorCode::kSucces;
    }

    return (code == LoRaErrorCode::kSucces);
}

bool UsrLg206PAdapter::End()
{
    return true;
}

int UsrLg206PAdapter::Available()
{
    return lora.Available();
}

size_t UsrLg206PAdapter::Read(uint8_t *buffer, size_t buffer_size)
{
    return lora.ReceiveMessage(buffer, buffer_size);
}

size_t UsrLg206PAdapter::Write(const uint8_t *message, const size_t message_size)
{
    return lora.SendMessage(message, message_size);
}

String UsrLg206PAdapter::GetId()
{
    if (this->id.length())
    {
        return this->id;
    }

    while (lora.BeginAtMode() != LoRaErrorCode::kSucces)
    {
        delay(1000);
    }

    while (lora.GetNodeId(this->id) != LoRaErrorCode::kSucces)
    {
        delay(1000);
    }

    while (lora.EndAtMode() != LoRaErrorCode::kSucces)
    {
        delay(1000);
    }

    return this->id;
}
