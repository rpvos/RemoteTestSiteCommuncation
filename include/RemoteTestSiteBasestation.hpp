#ifndef REMOTE_TEST_SITE_BASESTATION_HPP_
#define REMOTE_TEST_SITE_BASESTATION_HPP_

#include "IRemoteTestSiteController.hpp"
#include "RemoteTestSiteCommunicationAdapter.hpp"

class RemoteTestSiteBasestation : IRemoteTestSiteController
{
public:
    RemoteTestSiteBasestation();

    // TODO: think about the id giving system
    bool Join(uint64_t new_id) override;
    bool Measurement(RemoteTestSite_MeasurementType type, float value) override;
    bool UpdateFrequency(RemoteTestSite_MeasurementType type, RemoteTestSite_Timestamp frequency) override;
    bool Ping(uint64_t sender_id) override;
    bool Response(RemoteTestSite_ResponseCode response_code) override;
    bool UnknownMessageType(pb_size_t which_function_info) override;

    RemoteTestSiteCommunicationAdapter communication_adapter;

private:
};

#endif // #ifndef REMOTE_TEST_SITE_BASESTATION_HPP_