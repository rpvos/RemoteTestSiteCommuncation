#ifndef REMOTE_TEST_SITE_BASESTATION_HPP_
#define REMOTE_TEST_SITE_BASESTATION_HPP_

#include "interfaces/i_message_handler.hpp"
#include "interfaces/i_connection_handler.hpp"

class RemoteTestSiteBasestation : public IMessageHandler
{
public:
    RemoteTestSiteBasestation(IConnectionHandler *const connection_handler);

    void HandleMessage(const RemoteTestSite_Message &message) override;

private:
    IConnectionHandler *connection_handler;

    bool Syn(const RemoteTestSite_Message &message);
    bool Measurement(RemoteTestSite_MeasurementInfo type, int32_t value);
    bool UpdateFrequency(RemoteTestSite_MeasurementInfo type, RemoteTestSite_Timestamp frequency);
    bool Response(RemoteTestSite_ResponseCode response_code);
    bool UnknownMessageType(pb_size_t which_function_info);
};

#endif // #ifndef REMOTE_TEST_SITE_BASESTATION_HPP_