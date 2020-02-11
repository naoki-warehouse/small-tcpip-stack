#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "arp.h"
#include "eth.h"
#include "mbuf.h"

#define ARP_HARDWARE_TYPE_ETH (0x0001)
#define ARP_PROTOCOL_TYPE_IPV4 (0x0800)
#define ARP_HARDWARE_LENGTH_ETH (6)
#define ARP_PROTOCOL_LENGTH_IPV4 (4)
#define ARP_OPCODE_REQUEST (1)
#define ARP_OPCODE_REPLY (2)

struct arp_pkt {
    uint16_t hrd_type;
    uint16_t proto_type;

    uint8_t hrd_len;
    uint8_t proto_len;

    uint16_t opcode;

    uint8_t src_mac[6];
    uint8_t src_ip[4];
    uint8_t dst_mac[6];
    uint8_t dst_ip[4];
} __attribute__((packed));

int arp_init(){

}

int arp_rx(struct mbuf *buf){
    struct arp_pkt *pkt = (struct arp_pkt *)buf->payload;

    if(ntohs(pkt->hrd_type) != ARP_HARDWARE_TYPE_ETH) return -1;
    if(ntohs(pkt->proto_type) != ARP_PROTOCOL_TYPE_IPV4) return -1;
    if(pkt->hrd_len != ARP_HARDWARE_LENGTH_ETH) return -1;
    if(pkt->proto_len != ARP_PROTOCOL_LENGTH_IPV4) return -1;

    switch(ntohs(pkt->opcode)){
        case ARP_OPCODE_REQUEST:
            printf("ARP Request\n");
            buf->netdev = netdev_get_by_ip(pkt->dst_ip);
            for(int i=0;i<4;i++){
                printf("%d.", pkt->dst_ip[i]);
            }

            buf->payload = buf->data+14;
            memcpy(buf->hw_addr, pkt->src_mac, 6);
            memcpy(buf->ip_addr, pkt->src_ip, 4);
            memset(buf->data, 0, buf->dlen);
            if(buf->netdev != NULL) {
                printf("Found netdev entry\n");
                netdev_print(buf->netdev);
                arp_tx_reply(buf);
            }
            break;
        case ARP_OPCODE_REPLY:
            printf("ARP Reply\n");
            break;
        default:
            printf("Invalid opcode:%04X\n", ntohs(pkt->opcode));
            break;
    }

    return 0;
}

int arp_tx_reply(struct mbuf *buf){
    struct arp_pkt *pkt = (struct arp_pkt *)buf->payload;

    pkt->hrd_type = htons(ARP_HARDWARE_TYPE_ETH);
    pkt->proto_type = htons(ARP_PROTOCOL_TYPE_IPV4);
    pkt->hrd_len = ARP_HARDWARE_LENGTH_ETH;
    pkt->proto_len = ARP_PROTOCOL_LENGTH_IPV4;
    pkt->opcode = htons(ARP_OPCODE_REPLY);

    memcpy(pkt->src_mac, buf->netdev->hw_addr, 6);
    memcpy(pkt->src_ip, buf->netdev->ip_addr, 4);
    memcpy(pkt->dst_mac, buf->hw_addr, 6);
    memcpy(pkt->dst_ip, buf->ip_addr, 4);

    buf->plen = sizeof(struct arp_pkt);
    buf->payload = buf->data;
    eth_tx(buf, ETHERNET_TYPE_ARP);
}
