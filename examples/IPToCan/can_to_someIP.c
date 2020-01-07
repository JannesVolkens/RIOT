#include <stdio.h>
#include <string.h>
#include "can/can.h"

// Request ID, Protocol version, Interface version, Message type, Return Code
#define HEADERSIZE 8

//16 Byte header and at least 1 Byte and max 8 Byte data
#define MINBUFSIZE 17
#define MAXBUFSIZE 24

static uint32_t get_length(uint8_t dlc)
{
      return dlc + HEADERSIZE;
}

static void set_ID(uint8_t *buf, uint32_t id)
{
    buf[0] = (id & 0xff000000) >> 24;
    buf[1] = (id & 0x00ff0000) >> 16;
    buf[2] = (id & 0x0000ff00) >> 8;
    buf[3] = (id & 0x000000ff);
}

static void set_length(uint8_t *buf, uint32_t length)
{
    buf[4] = (length & 0xff000000) >> 24;
    buf[5] = (length & 0x00ff0000) >> 16;
    buf[6] = (length & 0x0000ff00) >> 8;
    buf[7] = (length & 0x000000ff);
}

static void set_client_id(uint8_t *buf, uint16_t clientID, uint16_t sessionID)
{
    buf[8] = (clientID & 0xff00) >> 8;
    buf[9] = (clientID & 0x00ff);
    buf[10] = (sessionID & 0xff00) >> 8;
    buf[11] = (sessionID & 0x00ff);
}

static void fill_payload(uint8_t *buf, const uint8_t *data, uint8_t dlc)
{
    for (int i = 16; i < (16 + dlc); i++) {
        buf[i] = data[i - 16];
    }
}

int can_to_someIP(const struct can_frame *frame, uint8_t *buf, uint8_t size)
{
    if (size < MINBUFSIZE || size > MAXBUFSIZE) {
        puts("Wrong buf size!");
        return -1;
    }
    set_ID(buf, frame->can_id);
    set_length(buf, get_length(frame->can_dlc));
    set_client_id(buf, 0xabcd, 0x0000);
    //Protoco, Interface, Message type, Return code
    buf[12] = 0x01;
    buf[13] = 0x00;
    buf[14] = 0x01;
    buf[15] = 0x00;

    fill_payload(buf, frame->data, frame->can_dlc);

    return 0;
}
