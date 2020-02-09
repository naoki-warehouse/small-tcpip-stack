#ifndef _MY_SOCK_H_
#define _MY_SOCK_H_

#include <stdint.h>

int my_sock_socket(int domain, int type, int protocol);

ssize_t my_sock_recv(int sockfd, void *buf, size_t len, int flags);
ssize_t my_sock_recvfrom();
ssize_t my_sock_recvmsg();

ssize_t my_sock_send();
ssize_t my_sock_sendto();
ssize_t my_sock_sendmsg();


#define MY_SOCK_PKT_SOCKET (0)
#define MY_SOCK_PKT_RECV (1)

#define MY_SOCK_PKT_OP_REQ (0)
#define MY_SOCK_PKT_OP_RES (1)

#define MY_SOCK_PKT_TYPE_RAW (0)

#define MY_SOCK_PKT_RES_OK (0)
#define MY_SOCK_PKT_RES_ERROR (1)

struct my_sock_pkt_socket_req {
    uint8_t pkt_type;
    uint8_t op;
    uint8_t sock_type;
};

struct my_sock_pkt_socket_res {
    uint8_t pkt_type;
    uint8_t op;
    uint8_t res;
};

#endif
