#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>

#include "mbuf.h"
#include "ipv4.h"
#include "icmp.h"


int icmp_rx(struct mbuf *buf){
    if(icmp_chksum(buf) != 0) return -1;

    struct icmp_pkt *pkt = (struct icmp_pkt *)buf->payload;

    switch(pkt->type){
        case ICMP_TYPE_ECHO:
            icmp_rx_echo(buf);
            break;
        case ICMP_TYPE_ECHO_REPLY:
            icmp_rx_echo_reply(buf);
            break;
        default:
            printf("Unknown ICMP Type:%d\n", pkt->type);
            mbuf_free_all(buf);
            return -1;
            break;
    }
    return 0;
}

int icmp_rx_echo(struct mbuf *buf){
    struct icmp_echo_pkt *pkt = (struct icmp_echo_pkt *)buf->payload;
    int header_len = buf->payload - buf->data;
    memset(buf->data, 0, header_len);

    int pkt_plen = buf->plen-(buf->payload-buf->data)-sizeof(struct icmp_pkt)-(sizeof(uint16_t)*2);
    printf("Recv ICMP ECHO id:%04X seq_num:%04X payload_len:%d\n", ntohs(pkt->id), ntohs(pkt->seq_num), pkt_plen);

    pkt->type = ICMP_TYPE_ECHO_REPLY;
    icmp_tx(buf);
}

int icmp_rx_echo_reply(struct mbuf *buf){
    printf("Recv ICMP ECHO REPLY\n");

}

int icmp_tx(struct mbuf *buf){
    struct icmp_pkt *pkt = (struct icmp_pkt *)buf->payload;
    pkt->chksum = 0;
    pkt->chksum = htons(icmp_chksum(buf));
    ipv4_tx(buf);
}

uint16_t icmp_chksum(struct mbuf *buf){
    uint8_t *pkt = buf->payload;
    int len = (buf->plen) - (buf->payload - buf->data);
    uint32_t chksum = 0;
    for(int i=0;i<(len/2);i++){
        chksum += (pkt[i*2] << 8)+pkt[i*2+1];
        if(chksum > 0xFFFF){
            chksum = (chksum&0xFFFF)+1;
        }
    }
    return ~chksum&0xFFFF;
}
