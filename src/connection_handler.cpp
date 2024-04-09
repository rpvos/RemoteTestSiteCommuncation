#include "connection_handler.hpp"
#include <stdlib.h> // stroll()

bool ConnectionHandler::IsConnected()
{
    return is_connected;
}

bool ConnectionHandler::GetId()
{
    String id = this->communication_device->GetId();
    uint64_t value = 0;
    for (size_t i = 0; i < id.length() && i < 8; i += 2)
    {
        String byte_string = id.substring(i, i + 2);
        uint8_t c = (uint8_t)strtoul(byte_string.c_str(), NULL, 16);
        value <<= 8;
        value |= c;
    }

    if (value)
    {
        this->proto_handler->SetId(value);
    }

    return value;
}

ConnectionHandler::ConnectionHandler(IMessageHandler *message_handler, ICommunicationDevice *communication_device, IProtoHandler *proto_handler)
{
    this->message_handler = message_handler;
    this->communication_device = communication_device;
    this->proto_handler = proto_handler;
    this->is_connected = false;
}

ConnectionHandler::~ConnectionHandler()
{
}

bool ConnectionHandler::Begin()
{
    return this->communication_device->Begin();
}

bool ConnectionHandler::Connect(const uint64_t destination)
{
    this->destination = destination;

    const size_t buffer_size = 128;
    uint8_t buffer[buffer_size] = {0};

    // Send SYN message
    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;
    message.which_function_info = RemoteTestSite_Message_syn_tag;

    size_t message_size = this->proto_handler->EncodeMessage(buffer, buffer_size, message);
    if (message_size <= 0)
    {

        Serial.println("Message encoding failed");

        return false;
    }

    size_t bytes_written = this->communication_device->Write(buffer, message_size);
    if (bytes_written <= 0)
    {
        Serial.println("Message could not be written to device");
        return false;
    }

    // Receive ACK message
    unsigned long now = millis();
    unsigned long time_out = 5000;
    while (!communication_device->Available() && now + time_out > millis())
    {
        delay(100);
    }

    if (!communication_device->Available())
    {

        Serial.println("No response");

        return false;
    }

    size_t size = communication_device->Read(buffer, buffer_size);
    RemoteTestSite_Message response = RemoteTestSite_Message_init_zero;
    bool succes = proto_handler->DecodeMessage(response, buffer, size);
    if (!succes)
    {

        Serial.println("Message decoding error");

        return false;
    }

    // Check acknowledgement
    if (response.has_acknowledge_number)
    {
        if (response.acknowledge_number != (uint32_t)(proto_handler->GetSequenceNumber() - 1))
        {
            Serial.println("Incorrect acknowledgement");
            Serial.println((int)response.acknowledge_number);
            Serial.println((int)proto_handler->GetSequenceNumber());

            return false;
        }
    }

    // Receive SYN message
    if (response.which_function_info == RemoteTestSite_Message_syn_tag)
    {
        // Send ACK message
        RemoteTestSite_Message ack_message = RemoteTestSite_Message_init_zero;
        ack_message.has_acknowledge_number = true;
        ack_message.acknowledge_number = response.sequence_number;
        ack_message.which_function_info = RemoteTestSite_Message_ack_tag;
        bool ack_is_send = Write(ack_message);
        // Can check for send confirmation
    }

    this->is_connected = true;
    return true;
}

bool ConnectionHandler::End()
{
    // Send FIN message

    // Receive ACK message
    // Receive FIN message

    // Send ACK message
    this->communication_device->End();
    is_connected = false;
    return true;
}

size_t ConnectionHandler::Available()
{
    // Check if messages are available
    return communication_device->Available();
}

bool ConnectionHandler::Read()
{
    const size_t buffer_size = 128;
    uint8_t buffer[buffer_size];
    size_t size = communication_device->Read(buffer, buffer_size);

    // Decode message
    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;
    auto succes = proto_handler->DecodeMessage(message, (const uint8_t *)buffer, size);

    if (!succes)
    {

        Serial.println("Read decode error");

        return false;
    }

    // Check CRC of message
    uint32_t crc = proto_handler->GetCrc(message, buffer, buffer_size);

    if (message.checksum != crc)
    {
        return false;
    }

    message_handler->HandleMessage(message);
    return true;
}

bool ConnectionHandler::Write(RemoteTestSite_Message message)
{
    const size_t buffer_size = 128;
    uint8_t buffer[buffer_size];

    size_t message_size = this->proto_handler->EncodeMessage(buffer, buffer_size, message);
    if (message_size <= 0)
    {

        Serial.println("Message encoding failed");

        return false;
    }
    size_t bytes_written = this->communication_device->Write(buffer, message_size);
    if (bytes_written <= 0)
    {

        Serial.println("Message could not be written to device");

        return false;
    }

    // TODO: maybe check for acknowledgement
    return true;
}

void ConnectionHandler::WaitForConnection()
{
    // TODO: implement for base station
    if (!communication_device->Available())
    {

        Serial.println("No new devices");

        delay(1000);
        return;
    }
    const size_t buffer_size = 256;
    uint8_t buffer[buffer_size];

    size_t size = communication_device->Read(buffer, buffer_size);

    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;
    bool succes = proto_handler->DecodeMessage(message, buffer, size);
    if (!succes)
    {
        // Decode message failed

        Serial.println("Decode failed");

        return;
    }

    uint32_t crc = proto_handler->GetCrc(message, buffer, buffer_size);

    if (crc != message.checksum)
    {

        Serial.println("Crc failed");

        return;
    }

    if (message.which_function_info == RemoteTestSite_Message_syn_tag)
    {
        // Return acknowledgement
        this->destination = message.sender_id;
        uint64_t acknowledgement_number = message.sequence_number;

        Serial.println("Sending acknowledgement");

        SendAcknowledgement(acknowledgement_number);

        // TODO: figure out what to do
        // Begin(destination);

        this->is_connected = true;
    }
}

bool ConnectionHandler::SendAcknowledgement(uint64_t acknowledgement_number)
{
    const size_t buffer_size = 128;
    uint8_t buffer[buffer_size];

    // Send ACK message
    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;
    message.which_function_info = RemoteTestSite_Message_ack_tag;
    message.has_acknowledge_number = true;
    message.acknowledge_number = acknowledgement_number;

    size_t message_size = this->proto_handler->EncodeMessage(buffer, buffer_size, message);
    if (message_size == 0)
    {

        Serial.println("Message encoding failed");

        return false;
    }
    size_t bytes_written = this->communication_device->Write(buffer, message_size);
    if (bytes_written == 0)
    {

        Serial.println("Message could not be written to device");

        return false;
    }

    return true;
}
