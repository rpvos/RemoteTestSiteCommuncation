#if !defined(PROTO_HELPER_HPP_)
#define PROTO_HELPER_HPP_

#include <stdint.h>
#include <pb.h>
#include <proto/base_message.pb.h>

class ProtoHelper
{
public:
    uint64_t id;
    uint64_t destination_id;
    uint64_t sequence_number;
    uint64_t acknowledge_number;

    ProtoHelper(uint64_t id = 0);
    ~ProtoHelper();

    uint64_t GetNewSequenceNumber();

    pb_ostream_t CreateOutputStream(uint8_t *const message_buffer, const size_t message_buffer_size);
    bool EncodeMeasurement(pb_ostream_t *const stream, const RemoteTestSite_MeasurementType type, const float value);
    bool EncodeUpdate(pb_ostream_t *const stream, const RemoteTestSite_MeasurementType type, const RemoteTestSite_Timestamp frequency);
    bool EncodeResponse(pb_ostream_t *const stream, const RemoteTestSite_ResponseCode code);
    bool EncodeUnionmessage(pb_ostream_t *const stream, const pb_msgdesc_t *messagetype, void *const message);
    bool EncodeBaseMessage(pb_ostream_t *const stream, RemoteTestSite_Message &message);
    bool AddCrc(uint8_t *buffer, const uint8_t buffer_size, RemoteTestSite_Message &message);
    uint32_t CalculateCrc(RemoteTestSite_Message &message, uint8_t *buffer, size_t buffer_size);

    pb_istream_t CreateInputStream(const uint8_t *const message_buffer, const size_t message_length);
};

#endif // PROTO_HELPER_HPP_
