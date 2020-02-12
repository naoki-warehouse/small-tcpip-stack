#ifndef _ETH_H_
#define _ETH_H_

#include "mbuf.h"

#define ETHERNET_TYPE_ARP (0x0806)
#define ETHERNET_TYPE_IP (0x0800)

struct eth_pkt {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t type;
} __attribute__((packed));


int eth_rx(struct mbuf *buf);
int eth_tx(struct mbuf *buf, uint16_t type);

#endif
