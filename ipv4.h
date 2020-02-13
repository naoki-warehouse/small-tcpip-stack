#ifndef _IPV4_H_
#define _IPV4_H_

#include <stdint.h>
#include "mbuf.h"

struct ipv4_pkt {
    uint8_t header_len : 4;
    uint8_t ver : 4;
    uint8_t tos;
    uint16_t total_len;
    uint16_t id;
    uint16_t fragment;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t chksum;
    uint8_t src_ip[4];
    uint8_t dst_ip[4];
} __attribute__((packed));

#define IPV4_PROTOCOL_ICMP (1)
#define IPV4_PROTOCOL_UDP (0x11)

int ipv4_rx(struct mbuf *buf);
int ipv4_tx(struct mbuf *buf);
uint16_t ipv4_chksum(struct mbuf *buf);


#endif
