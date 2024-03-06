#if !defined(CONNECTION_HANDLER_HPP_)
#define CONNECTION_HANDLER_HPP_

#include <stddef.h>
#include "proto/base_message.pb.h"
#include "interfaces/i_message_handler.hpp"
#include "interfaces/i_communication_device.hpp"
#include "interfaces/i_proto_handler.hpp"

/**
 * @brief
 * Connection handler used for beginning connection, sending data and ending connection.
 *
 */
class ConnectionHandler
{
private:
    IMessageHandler *message_handler;
    ICommunicationDevice *communication_device;
    IProtoHandler *proto_handler;
    uint64_t destination;
    bool is_connected;

public:
    ConnectionHandler(IMessageHandler *message_handler, ICommunicationDevice *communication_device, IProtoHandler *proto_handler);
    ~ConnectionHandler();

    bool IsConnected();
    bool GetId();
    bool Begin();
    bool Connect(const uint64_t destination);
    bool End();
    size_t Available();
    bool Read();
    bool Write(RemoteTestSite_Message message);
    void WaitForConnection();
    bool SendAcknowledgement(uint64_t acknowledgement_number);
};

#endif // CONNECTION_HANDLER_HPP_
