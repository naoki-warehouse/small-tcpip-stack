#ifndef _MBUF_H_
#define _MBUF_H_

#include <stdint.h>
#include "netdev.h"

struct mbuf {
    struct mbuf *next;
    struct mbuf *pre;
    struct netdev_info *netdev;
    int dlen;
    uint8_t *data;
};

struct mbuf* mbuf_alloc(int dlen);
int mbuf_add(struct mbuf* base, struct mbuf* new_buf);
int mbuf_free(struct mbuf* buf);
int mbuf_free_all(struct mbuf* buf);

#endif
