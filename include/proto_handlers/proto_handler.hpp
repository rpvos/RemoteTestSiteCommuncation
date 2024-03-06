#if !defined(PROTO_HANDLER_HPP_)
#define PROTO_HANDLER_HPP_

#include "proto/base_message.pb.h"
#include "interfaces/i_proto_handler.hpp"
#include "proto_handlers/proto_helper.hpp"

/**
 * @brief Output parameter
 *
 */
#define OUT

class ProtoHandler : public IProtoHandler, private ProtoHelper
{
private:
    bool need_acknoledgement;

public:
    ProtoHandler(uint64_t id = 0, uint32_t random_seed = 0);
    ~ProtoHandler();

    uint64_t GetSequenceNumber() override;
    bool SetId(uint64_t id) override;
    uint32_t GetCrc(RemoteTestSite_Message &message, uint8_t *buffer, size_t buffer_size) override;

    size_t EncodeMessage(OUT uint8_t *data, const size_t data_size, RemoteTestSite_Message message) override;
    bool DecodeMessage(OUT RemoteTestSite_Message &message, const uint8_t *data, const size_t data_length) override;
};

#endif // PROTO_HANDLER_HPP_
