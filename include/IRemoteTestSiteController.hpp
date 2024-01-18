#ifndef I_REMOTE_TEST_SITE_CONTROLLER_HPP_
#define I_REMOTE_TEST_SITE_CONTROLLER_HPP_

#include "proto/measurement_type.pb.h"
#include "proto/timestamp.pb.h"

/// @brief Interface for callbacks to handling of messages with target id of this module
class IRemoteTestSiteController
{
public:
    /// @brief Function to use when a message of type join is received
    /// @param new_id is the new id used for this module
    /// @return true if succesfull
    virtual bool Join(uint64_t new_id) = 0;

    /// @brief Function to use when a message of type measurement is received
    /// @param type of measurement
    /// @param value of measurement
    /// @return true if succesfull
    virtual bool Measurement(RemoteTestSite_MeasurementType type, float value);

    /// @brief Function to use when a message of type update frequency is received
    /// @param type of measurement
    /// @param frequency of measurement in timestamp
    /// @return true if succesfull
    virtual bool UpdateFrequency(RemoteTestSite_MeasurementType type, RemoteTestSite_Timestamp frequency) = 0;

    /// @brief Function to use when a message of type ping is received
    /// @param sender_id is the id from which the ping originated
    /// @return true if succesfull
    virtual bool Ping(uint64_t sender_id) = 0;

    /// @brief Function to use when a message of type response is received
    /// @param response_code of type ReponseCode
    /// @return true if succesfull
    virtual bool Response(ErrorCode response_code);

    /// @brief Function used when unknown message type is received
    /// @param which_function_info is the number of the message type
    /// @return true if succesfull
    virtual bool UnknownMessageType(pb_size_t which_function_info);
};

#endif // #ifndef I_REMOTE_TEST_SITE_CONTROLLER_HPP_