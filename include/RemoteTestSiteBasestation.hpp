#ifndef REMOTE_TEST_SITE_BASESTATION_HPP_
#define REMOTE_TEST_SITE_BASESTATION_HPP_

#include "IRemoteTestSiteController.hpp"
#include "RemoteTestSiteCommunicationAdapter.hpp"

class RemoteTestSiteBasestation : IRemoteTestSiteController
{
public:
    RemoteTestSiteBasestation();

    // TODO: think about the id giving system
    /// @brief Empty implementation because basestation does not need other id
    /// @param new_id
    /// @return
    bool Join(uint64_t new_id);

private:
    RemoteTestSiteCommunicationAdapter communication_adapter;
};

#endif // #ifndef REMOTE_TEST_SITE_BASESTATION_HPP_