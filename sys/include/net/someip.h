#ifndef NET_SOMEIP_H
#define NET_SOMEIP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* TODO define how big the payload should be */
#define SOMEIP_MAX_PLEN (8)
#define SOMEIP_HDR_SIZE (8)
#define SOMEIP_FULL_HDR_SIZE (16)

#define PROTOCOL_VERSION 0x01
#define INTERFACE_VERSION 0x03
#define MESSAGE_TYPE 0x02
#define RETURN_CODE 0x00

struct message_id {
    uint16_t service_id;
    uint16_t method_id;
};

struct request_id {
    uint16_t client_id;
    uint16_t session_id;
};

struct someip_hdr {
    struct message_id msg_id;
    uint32_t length;
    struct request_id rqst_id;
    uint8_t protocol_version;
    uint8_t interface_version;
    uint8_t msg_type;
    uint8_t return_code;
    uint8_t payload[SOMEIP_MAX_PLEN] __attribute__((aligned(8)));
};

#ifdef __cplusplus
}
#endif

#endif /* NET_SOMEIP_H */
