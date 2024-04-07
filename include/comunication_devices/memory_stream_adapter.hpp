#if !defined(MEMORY_STREAM_ADAPTER_HPP_)
#define MEMORY_STREAM_ADAPTER_HPP_

#include "interfaces/i_communication_device.hpp"
#include <Arduino.h>

class MemoryStreamAdapter : public ICommunicationDevice
{
private:
    String id;

public:
    bool Begin() override;
    bool End() override;
    int Available() override;
    size_t Read(uint8_t *buffer, size_t buffer_size) override;
    size_t Write(const uint8_t *message, const size_t message_size) override;
    string GetId() override;
};

#endif // MEMORY_STREAM_ADAPTER_HPP_
