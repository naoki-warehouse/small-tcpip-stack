#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "ipv4.h"
#include "mbuf.h"


static struct mbuf *buf_base = NULL;

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

    switch(pkt->protocol){
        case IPV4_PROTOCOL_ICMP:
            printf("Recv ICMP\n");
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

}
