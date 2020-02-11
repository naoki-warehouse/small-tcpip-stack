#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "eth.h"
#include "mbuf.h"
#include "netdev.h"

struct eth_pkt {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t type;
} __attribute__((packed));

static uint8_t brd_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#define ETHERNET_TYPE_ARP (0x0806)
#define ETHERNET_TYPE_IP (0x0800)

int eth_rx(struct mbuf* buf){
    if(buf->dlen < sizeof(struct eth_pkt)){
        fprintf(stderr, "Invalid Ethernet Frame\n");
        return -1;
    }

    struct eth_pkt *pkt = (struct eth_pkt *)buf->data;
    struct netdev_info* netdev = netdev_get_by_hw(pkt->dst_mac);
    if(netdev == NULL && memcmp(pkt->dst_mac, brd_mac, 6) != 0) return -1;

    uint8_t *dst = pkt->dst_mac;
    uint8_t *src = pkt->src_mac;
    printf("Recv ETH dst:%02X:%02X:%02X:%02X:%02X:%02X src:%02X:%02X:%02X:%02X:%02X:%02X type:%04X\n",
            dst[0], dst[1], dst[2], dst[3], dst[4], dst[5],
            src[0], src[1], src[2], src[3], src[4], src[5],
            ntohs(pkt->type));

    switch(ntohs(pkt->type)){
        case ETHERNET_TYPE_ARP:
            break;
        case ETHERNET_TYPE_IP:
            break;
        default:
            break;
    }
}
