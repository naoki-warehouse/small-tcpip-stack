#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "arp.h"
#include "eth.h"
#include "mbuf.h"
#include "config.h"
#include "arp_table.h"

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
    arp_table_init();
}

int arp_rx(struct mbuf *buf){
    struct arp_pkt *pkt = (struct arp_pkt *)buf->payload;

    if(ntohs(pkt->hrd_type) != ARP_HARDWARE_TYPE_ETH) return -1;
    if(ntohs(pkt->proto_type) != ARP_PROTOCOL_TYPE_IPV4) return -1;
    if(pkt->hrd_len != ARP_HARDWARE_LENGTH_ETH) return -1;
    if(pkt->proto_len != ARP_PROTOCOL_LENGTH_IPV4) return -1;

    if(ntohs(pkt->opcode) == ARP_OPCODE_REQUEST){
        buf->netdev = netdev_get_by_ip(pkt->dst_ip);
        if(buf->netdev == NULL){
            mbuf_free_all(buf);
            return -1;
        }

        uint8_t *mac = pkt->src_mac;
        uint8_t *ip = pkt->src_ip;
        printf("ARP Request src mac:%02X:%02X:%02X:%02X:%02X:%02X ip:%d.%d.%d.%d\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
                ip[0], ip[1], ip[2], ip[3]);
        buf->payload = buf->data+14;
        memcpy(buf->hw_addr, pkt->src_mac, 6);
        memcpy(buf->ip_addr, pkt->src_ip, 4);
        arp_table_add(pkt->src_mac, pkt->src_ip);
        arp_table_list();
        memset(buf->data, 0, buf->dlen);
        if(buf->netdev != NULL) {
            netdev_print(buf->netdev);
            arp_tx_reply(buf);
        }
    }else if(ntohs(pkt->opcode) == ARP_OPCODE_REPLY){
        buf->netdev = netdev_get_by_ip(pkt->dst_ip);
        if(buf->netdev == NULL){
            mbuf_free_all(buf);
            return -1;
        }
        uint8_t *mac = pkt->src_mac;
        uint8_t *ip = pkt->src_ip;
        printf("ARP Reply src mac:%02X:%02X:%02X:%02X:%02X:%02X ip:%d.%d.%d.%d\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
                ip[0], ip[1], ip[2], ip[3]);
        arp_table_add(pkt->src_mac, pkt->src_ip);
        mbuf_free_all(buf);
    }else{
        printf("Invalid opcode:%04X\n", ntohs(pkt->opcode));
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

int arp_tx_request(struct mbuf *buf){
    struct arp_pkt *pkt = (struct arp_pkt *)(buf->data+14);

    pkt->hrd_type = htons(ARP_HARDWARE_TYPE_ETH);
    pkt->proto_type = htons(ARP_PROTOCOL_TYPE_IPV4);
    pkt->hrd_len = ARP_HARDWARE_LENGTH_ETH;
    pkt->proto_len = ARP_PROTOCOL_LENGTH_IPV4;
    pkt->opcode = htons(ARP_OPCODE_REQUEST);

    memcpy(pkt->src_mac, buf->netdev->hw_addr, 6);
    memcpy(pkt->src_ip, buf->netdev->ip_addr, 4);
    memcpy(pkt->dst_mac, buf->hw_addr, 6);
    memcpy(pkt->dst_ip, buf->ip_addr, 4);

    buf->plen = sizeof(struct arp_pkt);
    buf->payload = buf->data;
    eth_tx(buf, ETHERNET_TYPE_ARP);
}
