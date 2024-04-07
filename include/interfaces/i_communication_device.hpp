#if !defined(I_COMMUNICATION_DEVICE_HPP_)
#define I_COMMUNICATION_DEVICE_HPP_

#include <stddef.h>
#include <stdint.h>
#include <Arduino.h>

class ICommunicationDevice
{
private:
public:
    virtual bool Begin() = 0;
    virtual bool End() = 0;
    virtual int Available() = 0;
    virtual size_t Read(uint8_t *buffer, size_t buffer_size) = 0;
    virtual size_t Write(const uint8_t *message, const size_t message_size) = 0;
    virtual String GetId() = 0;
};

#endif // I_COMMUNICATION_DEVICE_HPP_
