#include <stdio.h>
#include <string.h>

#include "can/can.h"
#include "can/conn/raw.h"
#include "can/conn/isotp.h"
#include "can/device.h"

uint8_t dlc = 0;

void set_ID(uint8_t *buf, struct can_frame *frame)
{
    uint32_t id = buf[3] | buf[2] << 8 | buf[1] << 16 | buf[0] << 24;
    if (id > CAN_SFF_MASK) {
        id = id | CAN_EFF_FLAG;
    }

    frame->can_id = id;
}

void set_data(uint8_t *buf, struct can_frame *frame)
{
    for (int i = 0; i < dlc; i++) {
        frame->data[i] = buf[16 + i];
    }
}

void someIP_to_can(uint8_t *buf, uint32_t size)
{
    dlc = size - 16;
    struct can_frame frame;

    int ifnum = 0;

    set_ID(buf, &frame);
    frame.can_dlc = dlc;
    set_data(buf, &frame);

    conn_can_raw_t conn;
    conn_can_raw_create(&conn, NULL, 0, ifnum, 0);
    int ret = conn_can_raw_send(&conn, &frame, 0);
    if (ret < 0) {
        puts("Error when trying to send");
    }
}
