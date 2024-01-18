#ifndef REMOTE_TEST_SITE_SENSOR_HPP_
#define REMOTE_TEST_SITE_SENSOR_HPP_

#include "IRemoteTestSiteController.hpp"
#include "RemoteTestSiteCommunicationAdapter.hpp"

class RemoteTestSiteSensorNode : IRemoteTestSiteController
{
public:
    RemoteTestSiteSensorNode() : communication_adapter(this);

    /// @brief Implementation for setting id to new id
    /// @param new_id is used from now on by this node
    /// @return true if succesfull
    bool Join(uint64_t new_id);

    RemoteTestSiteCommunicationAdapter communication_adapter;
};

#endif // #ifndef REMOTE_TEST_SITE_SENSOR_HPP_