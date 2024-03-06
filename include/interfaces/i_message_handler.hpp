#if !defined(I_MESSAGE_HANDLER_HPP_)
#define I_MESSAGE_HANDLER_HPP_

#include "proto/base_message.pb.h"

class IMessageHandler
{
private:
public:
    virtual void HandleMessage(RemoteTestSite_Message message) = 0;
};

#endif // I_MESSAGE_HANDLER_HPP_
