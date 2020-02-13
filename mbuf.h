#ifndef _MBUF_H_
#define _MBUF_H_

#include <stdint.h>
#include "netdev.h"

struct mbuf {
    struct mbuf *next_pkt;
    struct mbuf *prev_pkt;
    struct mbuf *next;
    struct mbuf *pre;
    struct netdev_info *netdev;
    uint8_t hw_addr[6];
    uint8_t ip_addr[4];
    int dlen;
    int plen;
    uint8_t *data;
    uint8_t *payload;
};

struct mbuf* mbuf_alloc(int dlen);
int mbuf_add(struct mbuf* base, struct mbuf* new_buf);
int mbuf_free(struct mbuf* buf);
int mbuf_free_all(struct mbuf* buf);

#endif
