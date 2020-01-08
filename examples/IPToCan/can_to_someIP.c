#include <stdio.h>
#include <string.h>
#include "can/can.h"
#include "net/someip.h"
#include "byteorder.h"

// Request ID, Protocol version, Interface version, Message type, Return Code
#define HEADERSIZE 8
#define PROTOCOL_VERSION 0x01
#define INTERFACE_VERSION 0x03
#define MESSAGE_TYPE 0x02
#define RETURN_CODE 0x00

static void set_msg_id(struct message_id *msg_id, uint32_t can_id)
{
    msg_id->service_id = htons((0xFFFF0000 & can_id) >> 16);
    msg_id->method_id = htons((0x0000FFFF & can_id));
}

static uint32_t set_length(uint8_t dlc)
{
    return HEADERSIZE + (uint32_t)dlc;
}

static void set_request_id(struct request_id *rqst_id)
{
    /* TODO define how request ID should be set */
    // rqst_id->client_id = htons((0xFFFF0000 & can_id) >> 16);
    // rqst_id->session_id = htons((0x0000FFFF & can_id));
    rqst_id->client_id = 0x00;
    rqst_id->session_id = 0x00;

}

static void fill_payload(uint8_t *payload, const uint8_t *data, uint8_t dlc)
{
    for (size_t i = 0; i < dlc; i++) {
        payload[i] = data[i];
    }
}

void can_to_someIP(const struct can_frame *frame, struct someip_hdr *hdr)
{
    set_msg_id(&hdr->msg_id, frame->can_id);
    hdr->length = htonl(set_length(frame->can_dlc));
    set_request_id(&hdr->rqst_id);

    hdr->protocol_version = PROTOCOL_VERSION;
    hdr->interface_version = INTERFACE_VERSION;
    hdr->msg_type = MESSAGE_TYPE;
    hdr->return_code = RETURN_CODE;

    fill_payload(hdr->payload, frame->data, frame->can_dlc);
}
