#include <stdio.h>
#include <string.h>
#include "can/can.h"
#include "net/someip.h"
#include "byteorder.h"

struct someip_hdr hdr;

static void set_msg_id(uint32_t can_id)
{
    /* TODO define how the message ID should be set */
    hdr.msg_id.service_id = htons((0xFFFF0000 & can_id) >> 16);
    hdr.msg_id.method_id = htons((0x0000FFFF & can_id));
}

static uint32_t set_length(uint8_t dlc)
{
    return SOMEIP_HDR_SIZE + (uint32_t)dlc;
}

static void set_request_id(void)
{
    /* TODO define how the request ID should be set */
    hdr.rqst_id.client_id = 0x00;
    hdr.rqst_id.session_id = 0x00;

}

static void fill_payload(const uint8_t *data, uint8_t dlc)
{
    for (size_t i = 0; i < dlc; i++) {
        hdr.payload[i] = data[i];
    }
}

struct someip_hdr can_to_someIP(const struct can_frame *frame)
{
    set_msg_id(frame->can_id);
    hdr.length = htonl(set_length(frame->can_dlc));
    set_request_id();

    hdr.protocol_version = PROTOCOL_VERSION;
    hdr.interface_version = INTERFACE_VERSION;
    hdr.msg_type = MESSAGE_TYPE;
    hdr.return_code = RETURN_CODE;

    fill_payload(frame->data, frame->can_dlc);

    return hdr;
}
