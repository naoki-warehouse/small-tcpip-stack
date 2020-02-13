#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

#include "eth.h"
#include "ipv4.h"
#include "icmp.h"
#include "mbuf.h"


static struct mbuf *buf_base = NULL;

static uint16_t ipv4_pkt_id = 0;

int ipv4_rx(struct mbuf *buf){
    struct ipv4_pkt *pkt = (struct ipv4_pkt *)buf->payload;
    if(pkt->ver != 4){
        mbuf_free_all(buf);
        return -1;
    }
    uint8_t *src = pkt->src_ip;
    uint8_t *dst = pkt->dst_ip;
    uint8_t fragment_flag = (ntohs(pkt->fragment) >> 13)&0x1;
    fprintf(stdout, "Recv ipv4 src:%d.%d.%d.%d dst:%d.%d.%d.%d\n",
            src[0], src[1], src[2], src[3], dst[0], dst[1], dst[2], dst[3]);
    fprintf(stdout, "          TotalLen:%d ID:%04X FragmentFlag:%d TTL:%d\n",
            ntohs(pkt->total_len), ntohs(pkt->id), fragment_flag, pkt->ttl);
    buf->payload = buf->payload + (pkt->header_len*4);
    memcpy(buf->ip_addr, pkt->src_ip, 4);

    switch(pkt->protocol){
        case IPV4_PROTOCOL_ICMP:
            icmp_rx(buf);
            break;
        case IPV4_PROTOCOL_UDP:
            printf("Recv UDP\n");
            break;
        default:
            printf("Unknown protocol:%d\n", pkt->protocol);
            break;
    }
}

int ipv4_tx(struct mbuf *buf){
    buf->payload = buf->payload-20;
    if(buf->payload < buf->data) return -1;

    struct ipv4_pkt *pkt = (struct ipv4_pkt *)buf->payload;
    pkt->header_len = 5;
    pkt->ver = 4;
    pkt->tos = 0;
    pkt->total_len = ntohs(buf->plen-(buf->payload-buf->data));
    pkt->id = ntohs(ipv4_pkt_id);
    pkt->fragment = 0;
    pkt->ttl = 64;
    pkt->protocol = IPV4_PROTOCOL_ICMP;
    pkt->chksum = 0;
    memcpy(pkt->src_ip, buf->netdev->ip_addr, 4);
    memcpy(pkt->dst_ip, buf->ip_addr, 4);
    pkt->chksum = ntohs(ipv4_chksum(buf));
    eth_tx(buf, ETHERNET_TYPE_IP);
    ipv4_pkt_id++;
}

uint16_t ipv4_chksum(struct mbuf *buf){
  int len = 5*2;
  uint8_t *pkt = buf->payload;
  uint32_t chksum = 0;

  for(int i=0;i<len;i++){
    chksum += (pkt[i*2] << 8)+pkt[i*2+1];
    if(chksum > 0xFFFF){
      chksum = (chksum&0xFFFF)+1;
    }
  }
  return ~chksum&0xFFFF;
}
