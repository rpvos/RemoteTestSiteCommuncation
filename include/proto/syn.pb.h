/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8 */

#ifndef PB_REMOTETESTSITE_PROTO_SYN_PB_H_INCLUDED
#define PB_REMOTETESTSITE_PROTO_SYN_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
typedef struct _RemoteTestSite_Syn {
    char dummy_field;
} RemoteTestSite_Syn;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define RemoteTestSite_Syn_init_default          {0}
#define RemoteTestSite_Syn_init_zero             {0}

/* Field tags (for use in manual encoding/decoding) */

/* Struct field encoding specification for nanopb */
#define RemoteTestSite_Syn_FIELDLIST(X, a) \

#define RemoteTestSite_Syn_CALLBACK NULL
#define RemoteTestSite_Syn_DEFAULT NULL

extern const pb_msgdesc_t RemoteTestSite_Syn_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define RemoteTestSite_Syn_fields &RemoteTestSite_Syn_msg

/* Maximum encoded size of messages (where known) */
#define REMOTETESTSITE_PROTO_SYN_PB_H_MAX_SIZE   RemoteTestSite_Syn_size
#define RemoteTestSite_Syn_size                  0

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
