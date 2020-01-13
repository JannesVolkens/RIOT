#include <stdio.h>
#include "net/sock/udp.h"
#include "thread.h"
#include "net/someip.h"

// 4 bytes ID, 1 byte DLC, 1 <= size >= 8 Data
#define BUFSIZE(size) (5 + size)

sock_udp_t sock;
uint16_t port = 0;

char rcv_thread_stack[THREAD_STACKSIZE_MAIN];

extern void someIP_to_can(uint8_t *buf, uint32_t size);

static void *_receive(void *arg)
{
    (void)arg;
    uint8_t buf[24];

    uint32_t length;

    while (1) {
        sock_udp_ep_t remote;

        if (sock_udp_recv(&sock, buf, sizeof(buf), SOCK_NO_TIMEOUT, &remote) >= 0) {
            puts("MSG received");
        }
        length = buf[7] | buf[6] << 8 | buf[5] << 16 | buf[4] << 24;

        for (size_t i = 0; i < (length+8); i++) {
            printf("BUF[%d]: %x\n", i, buf[i]);
        }

        // someIP_to_can(buf, 8 + length);
    }

    return NULL;
}

static int _start_socket(uint16_t port)
{
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = port;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
    puts("Error creating UDP sock");

    return 1;
    }

    thread_create(rcv_thread_stack, sizeof(rcv_thread_stack), THREAD_PRIORITY_MAIN - 1,
    THREAD_CREATE_STACKTEST, _receive, NULL, "rcv_thread");

    puts("Socket created and recv started");

    return 0;
}

int _send_udp_sock(struct someip_hdr *data, int size)
{
    sock_udp_ep_t remote = { .family = AF_INET6 };
    remote.port = port;
    remote.addr.ipv6[0] = 1;

    ipv6_addr_set_all_nodes_multicast((ipv6_addr_t *)&remote.addr.ipv6,
                                  IPV6_ADDR_MCAST_SCP_LINK_LOCAL);

    if (sock_udp_send(&sock, data, size, &remote) < 0) {
        puts("Error sending message");
        sock_udp_close(&sock);
        return 1;
    }

    puts("Send msg");

    return 0;
}

static int _send(void)
{
    sock_udp_ep_t remote = { .family = AF_INET6 };
    remote.port = port;
    remote.addr.ipv6[0] = 1;

    uint8_t data[22];

    //MSGID
    data[0] = 0x00;
    data[1] = 0x00;
    data[2] = 0x03;
    data[3] = 0xFC;

    //LENGTH
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0x00;
    data[7] = 0x0E;

    //REQUEST
    data[8] = 0xab;
    data[9] = 0xcd;
    data[10] = 0x00;
    data[11] = 0x00;

    //Protocol/Interface/MSGType/Return
    data[12] = 0x01;
    data[13] = 0x03;
    data[14] = 0x02;
    data[15] = 0x00;

    //PAYLOAD
    data[16] = 0x01;
    data[17] = 0x00;
    data[18] = 0xFF;
    data[19] = 0xA1;
    data[20] = 0x10;
    data[21] = 0xFB;

    ipv6_addr_set_all_nodes_multicast((ipv6_addr_t *)&remote.addr.ipv6,
                                  IPV6_ADDR_MCAST_SCP_LINK_LOCAL);

    if (sock_udp_send(&sock, data, sizeof(data), &remote) < 0) {
        puts("Error sending message");
        sock_udp_close(&sock);
        return 1;
    }

    puts("Send msg");

    return 0;
}

static void _set_port(uint16_t new_port)
{
    port = new_port;
    puts("Dst-Port set");
}

int udp_sock_cmd(int argc, char **argv)
{
    if (argc < 2) {
        puts("udp_sock [start/send/port]");
        return 1;
    }
    if (strcmp(argv[1], "start") == 0) {
          if (argc < 3) {
              puts("udp_sock start [port]");
              return 1;
          }
          _start_socket(atoi(argv[2]));
    } else if (strcmp(argv[1], "send") == 0) {
          _send();
    } else if (strcmp(argv[1], "port") == 0) {
          if (argc < 3) {
              puts("udp_sock port [port]");
              return 1;
          }
          _set_port(atoi(argv[2]));
    }

    return 0;
}
