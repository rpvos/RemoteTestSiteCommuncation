/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8 */

#ifndef PB_REMOTETESTSITE_PROTO_UPDATE_PB_H_INCLUDED
#define PB_REMOTETESTSITE_PROTO_UPDATE_PB_H_INCLUDED
#include <pb.h>
#include "proto/measurement_type.pb.h"
#include "proto/timestamp.pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
typedef struct _RemoteTestSite_UpdateFrequency {
    bool has_type;
    RemoteTestSite_MeasurementType type;
    bool has_frequency;
    RemoteTestSite_Timestamp frequency;
} RemoteTestSite_UpdateFrequency;

typedef struct _RemoteTestSite_Update {
    pb_size_t which_UpdateType;
    union {
        RemoteTestSite_UpdateFrequency update_frequency;
    } UpdateType;
} RemoteTestSite_Update;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define RemoteTestSite_UpdateFrequency_init_default {false, _RemoteTestSite_MeasurementType_MIN, false, RemoteTestSite_Timestamp_init_default}
#define RemoteTestSite_Update_init_default       {0, {RemoteTestSite_UpdateFrequency_init_default}}
#define RemoteTestSite_UpdateFrequency_init_zero {false, _RemoteTestSite_MeasurementType_MIN, false, RemoteTestSite_Timestamp_init_zero}
#define RemoteTestSite_Update_init_zero          {0, {RemoteTestSite_UpdateFrequency_init_zero}}

/* Field tags (for use in manual encoding/decoding) */
#define RemoteTestSite_UpdateFrequency_type_tag  1
#define RemoteTestSite_UpdateFrequency_frequency_tag 2
#define RemoteTestSite_Update_update_frequency_tag 1

/* Struct field encoding specification for nanopb */
#define RemoteTestSite_UpdateFrequency_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, UENUM,    type,              1) \
X(a, STATIC,   OPTIONAL, MESSAGE,  frequency,         2)
#define RemoteTestSite_UpdateFrequency_CALLBACK NULL
#define RemoteTestSite_UpdateFrequency_DEFAULT NULL
#define RemoteTestSite_UpdateFrequency_frequency_MSGTYPE RemoteTestSite_Timestamp

#define RemoteTestSite_Update_FIELDLIST(X, a) \
X(a, STATIC,   ONEOF,    MESSAGE,  (UpdateType,update_frequency,UpdateType.update_frequency),   1)
#define RemoteTestSite_Update_CALLBACK NULL
#define RemoteTestSite_Update_DEFAULT NULL
#define RemoteTestSite_Update_UpdateType_update_frequency_MSGTYPE RemoteTestSite_UpdateFrequency

extern const pb_msgdesc_t RemoteTestSite_UpdateFrequency_msg;
extern const pb_msgdesc_t RemoteTestSite_Update_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define RemoteTestSite_UpdateFrequency_fields &RemoteTestSite_UpdateFrequency_msg
#define RemoteTestSite_Update_fields &RemoteTestSite_Update_msg

/* Maximum encoded size of messages (where known) */
#define REMOTETESTSITE_PROTO_UPDATE_PB_H_MAX_SIZE RemoteTestSite_Update_size
#define RemoteTestSite_UpdateFrequency_size      26
#define RemoteTestSite_Update_size               28

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
