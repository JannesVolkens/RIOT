#include <stdio.h>
#include "net/sock/udp.h"
#include "thread.h"
uint8_t buf[128];
sock_udp_t sock;

char rcv_thread_stack[THREAD_STACKSIZE_MAIN];

static void *_receive(void *arg)
{
    (void)arg;

    while (1) {
        sock_udp_ep_t remote;
        ssize_t res;

        if ((res = sock_udp_recv(&sock, buf, sizeof(buf), SOCK_NO_TIMEOUT, &remote)) >= 0) {
            puts("received MSG");

            for (int i = 0; i < res; i++) {
                printf("%c", buf[i]);
            }
            printf("\n");
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

int _send(uint16_t port)
{
    sock_udp_ep_t remote = { .family = AF_INET6 };
    remote.port = port;
    remote.addr.ipv6[0] = 1;

    ipv6_addr_set_all_nodes_multicast((ipv6_addr_t *)&remote.addr.ipv6,
                                  IPV6_ADDR_MCAST_SCP_LINK_LOCAL);

    if (sock_udp_send(&sock, "HELLO", sizeof("HELLO"), &remote) < 0) {
        puts("Error sending message");
        sock_udp_close(&sock);
        return 1;
    }

    puts("Send msg");

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
          _send(atoi(argv[2]));
    }

    return 0;
}
