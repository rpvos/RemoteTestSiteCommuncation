#if !defined(I_PROTO_HANDLER_HPP_)
#define I_PROTO_HANDLER_HPP_

#include <stdint.h>
#include <stddef.h>
#include "proto/base_message.pb.h"

#define OUT

class IProtoHandler
{
public:
    virtual uint64_t GetSequenceNumber() = 0;

    virtual bool SetId(uint64_t id) = 0;
    virtual uint32_t GetCrc(RemoteTestSite_Message &message, uint8_t *buffer, size_t buffer_size) = 0;
    virtual size_t EncodeMessage(OUT uint8_t *data, const size_t data_size, RemoteTestSite_Message message) = 0;
    virtual bool DecodeMessage(OUT RemoteTestSite_Message &message, const uint8_t *data, const size_t data_length) = 0;
};

#endif // I_PROTO_HANDLER_HPP_
