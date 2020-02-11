#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "mbuf.h"

struct mbuf* mbuf_alloc(int dlen){
    struct mbuf *buf = (struct mbuf *)malloc(sizeof(struct mbuf));
    if(buf == NULL){
        fprintf(stderr, "Failed to allocate mbuf\n");
        return NULL;
    }

    buf->next = NULL;
    buf->pre = NULL;
    buf->netdev = NULL;
    buf->dlen = dlen;
    buf->data = malloc(dlen);
    if(buf->data == NULL){
        fprintf(stderr, "Failed to allocate mbuf data\n");
        free(buf);
        return NULL;
    }

    return buf;
}

int mbuf_free(struct mbuf* buf){

}

int mbuf_free_all(struct mbuf* buf){
    struct mbuf *mbuf_now = buf;
    while(buf->next != NULL){
        buf = buf->next;
    }
    while(buf->pre != NULL){
        buf = buf->pre;
        free(buf->next);
    }
    free(buf);
}
