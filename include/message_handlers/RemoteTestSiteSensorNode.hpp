#ifndef REMOTE_TEST_SITE_SENSOR_NODE_HPP_
#define REMOTE_TEST_SITE_SENSOR_NODE_HPP_

#include "interfaces/i_message_handler.hpp"

class RemoteTestSiteSensorNode : public IMessageHandler
{
public:
    void HandleMessage(RemoteTestSite_Message message) override;

private:
    bool Measurement(RemoteTestSite_MeasurementInfo type, int32_t value);
    bool UpdateFrequency(RemoteTestSite_MeasurementInfo type, RemoteTestSite_Timestamp frequency);
    bool Response(RemoteTestSite_ResponseCode response_code);
    bool UnknownMessageType(pb_size_t which_function_info);
};

#endif // #ifndef REMOTE_TEST_SITE_SENSOR_NODE_HPP_
