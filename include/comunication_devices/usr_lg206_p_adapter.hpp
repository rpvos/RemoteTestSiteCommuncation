#if !defined(USR_LG206_P_ADAPTER_HPP_)
#define USR_LG206_P_ADAPTER_HPP_

#include "interfaces/i_communication_device.hpp"
#include <usr_lg206_p.h>

class UsrLg206PAdapter : public ICommunicationDevice
{
private:
    RS485 rs = RS485(2, 2, &Serial1);
    UsrLg206P lora = UsrLg206P(&rs);
    String id;

public:
    UsrLg206PAdapter();
    ~UsrLg206PAdapter();

    bool Begin() override;
    bool End() override;
    int Available() override;
    size_t Read(uint8_t *buffer, size_t buffer_size) override;
    size_t Write(const uint8_t *message, const size_t message_size) override;
    String GetId() override;
};

#endif // USR_LG206_P_ADAPTER_HPP_
