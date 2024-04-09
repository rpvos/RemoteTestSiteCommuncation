#if !defined(I_CONNECTION_HANDLER_HPP_)
#define I_CONNECTION_HANDLER_HPP_

#include "proto/base_message.pb.h"

class IConnectionHandler
{
public:
    virtual size_t Available() = 0;
    virtual bool Read() = 0;
    virtual bool Write(RemoteTestSite_Message message) = 0;
};

#endif // I_CONNECTION_HANDLER_HPP_
