#ifndef REMOTE_TEST_SITE_COMMUNICATION_HPP_
#define REMOTE_TEST_SITE_COMMUNICATION_HPP_

#include <proto/base_message.pb.h>
#include "IRemoteTestSiteController.hpp"

class RemoteTestSiteCommunicationAdapter
{
public:
    RemoteTestSiteCommunicationAdapter(IRemoteTestSiteController *const controller);

    pb_ostream_t CreateOutputStream(uint8_t *const message_buffer, const size_t message_buffer_size);
    bool EncodeJoin(pb_ostream_t *const stream, const uint64_t destination_id, uint64_t new_id);
    bool EncodeMeasurement(pb_ostream_t *const stream, const uint64_t destination_id, const RemoteTestSite_MeasurementType type, const float value);
    bool EncodeUpdate(pb_ostream_t *const stream, const uint64_t destination_id, const RemoteTestSite_MeasurementType type, const RemoteTestSite_Timestamp frequency);
    bool EncodePing(pb_ostream_t *const stream, const uint64_t destination_id);
    bool EncodeResponse(pb_ostream_t *const stream, const uint64_t destination_id, const RemoteTestSite_ResponseCode code);

    pb_istream_t CreateInputStream(const uint8_t *const message_buffer, const size_t message_length);
    bool HandleMessage(pb_istream_t *const stream);

    void SetId(uint64_t new_id);

private:
    uint64_t id;
    IRemoteTestSiteController *controller;

    bool EncodeUnionmessage(pb_ostream_t *const stream, const pb_msgdesc_t *messagetype, void *const message);

    bool EncodeBaseMessage(pb_ostream_t *const stream, const uint64_t destination_id);
    void HandleMessage(const RemoteTestSite_Message message);
};

#endif