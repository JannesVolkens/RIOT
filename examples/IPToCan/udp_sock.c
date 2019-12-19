#include <stdio.h>
#include "net/sock/udp.h"
#include "thread.h"

// 4 bytes ID, 1 byte DLC, 1 <= size >= 8 Data
#define BUFSIZE(size) (5 + size)

sock_udp_t sock;

char rcv_thread_stack[THREAD_STACKSIZE_MAIN];

static void *_receive(void *arg)
{
    (void)arg;
    uint8_t buf[13];

    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];

    while (1) {
        sock_udp_ep_t remote;

        if (sock_udp_recv(&sock, buf, sizeof(buf), SOCK_NO_TIMEOUT, &remote) >= 0) {
            puts("received MSG");
            id = buf[0] | buf[1] << 8 | buf[2] << 16 | buf[3] << 24;
            dlc = buf[4];
            printf("ID: %lx\nDLC: %d\n", id, dlc);
            int test = 0;
            for (int i = 5; i < (5 + dlc); i++) {
                data[test] = buf[i];
                test++;
            }
            for (int i = 0; i < dlc; i++) {
                printf("DATA[%d]: %x\n", i, data[i]);
            }
        }
    }

    return NULL;
}

static int start_socket(uint16_t port)
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

void set_ID(uint8_t *buf, uint32_t id)
{
    buf[0] = (id & 0x000000ff);
    buf[1] = (id & 0x0000ff00) >> 8;
    buf[2] = (id & 0x00ff0000) >> 16;
    buf[3] = (id & 0xff000000) >> 24;
}

int _send(uint16_t port, uint32_t id, uint8_t dlc, uint8_t *data)
{
    sock_udp_ep_t remote = { .family = AF_INET6 };
    remote.port = port;
    remote.addr.ipv6[0] = 1;

    uint8_t buf[BUFSIZE(dlc)];

    set_ID(buf, id);
    buf[4] = dlc;

    int test = 5;
    for (int i = 0; i < dlc; i++) {
        buf[test] = data[i];
        test++;
    }

    ipv6_addr_set_all_nodes_multicast((ipv6_addr_t *)&remote.addr.ipv6,
                                  IPV6_ADDR_MCAST_SCP_LINK_LOCAL);

    if (sock_udp_send(&sock, buf, sizeof(buf), &remote) < 0) {
        puts("Error sending message");
        sock_udp_close(&sock);
        return 1;
    }

    puts("Send msg");
    memset(buf, 0, sizeof(buf));

    return 0;
}

int udp_sock_cmd(int argc, char **argv)
{
    if (argc < 2) {
        puts("udp_sock [start/send] [port]");
        return 1;
    }
    if (strcmp(argv[1], "start") == 0) {
          if (argc < 3) {
              puts("udp_sock start [port]");
              return 1;
          }
          start_socket(atoi(argv[2]));
    } else if (strcmp(argv[1], "send") == 0) {
          //_send(atoi(argv[2]));
    }

    return 0;
}
