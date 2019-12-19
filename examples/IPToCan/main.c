/*
 * Copyright (C) 2015 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Example application for demonstrating the RIOT network stack
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "shell.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

extern int _can_handler(int argc, char **argv);
extern int _can_trx_handler(int argc, char **argv);
extern int udp_sock_cmd(int argc, char **argv);
extern int _send(uint16_t port, uint32_t id, uint8_t dlc, uint8_t *data);

void can_to_udp_sock(uint32_t id, uint8_t dlc, uint8_t *data)
{
    uint16_t port = 8808;
    _send(port, id, dlc, data);
}

static const shell_command_t shell_commands[] = {
    { "test_can", "CAN test functions", _can_handler},
    { "can_trx", "can_trx functions", _can_trx_handler},
    { "udp_sock", "socket functions", udp_sock_cmd},
    { NULL, NULL, NULL }
};

int main(void)
{
    /* we need a message queue for the thread running the shell in order to
     * receive potentially fast incoming networking packets */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("RIOT network stack example application");

    /* start shell */
    puts("All up, running the shell now");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* should be never reached */
    return 0;
}
