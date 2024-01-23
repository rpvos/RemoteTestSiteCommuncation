#include "RemoteTestSiteSensorNode.hpp"

RemoteTestSiteSensorNode::RemoteTestSiteSensorNode() : communication_adapter(this)
{
}

bool RemoteTestSiteSensorNode::Join(uint64_t new_id)
{
    // New id must be bigger then 1 (Basestation id)
    if (new_id > 1)
    {
        this->communication_adapter.SetId(new_id);
        return true;
    }

    return false;
}
