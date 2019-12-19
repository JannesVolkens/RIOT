#include <stdio.h>
#include "can/can.h"

// Request ID, Protocol version, Interface version, Message type, Return Code
#define HEADERSIZE 8

#define BUFSIZE(size) (8 + size)

static uint32_t get_length(uint8_t dlc)
{
      return dlc + HEADERSIZE;
}

void can_to_someIP(const struct can_frame *frame)
{
    printf("ID: %lx\nDLC: %d\n", frame->can_id, frame->can_dlc);
    for (int i = 0; i < frame->can_dlc; i++) {
        printf("DATA[%d]: %x\n", i, frame->data[i]);
    }

    uint8_t buf[BUFSIZE(get_length(frame->can_dlc))];

    buf[0] = frame->can_id;

    printf("SIZE: %d\n", sizeof(buf));
}
