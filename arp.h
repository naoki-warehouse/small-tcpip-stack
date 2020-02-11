#ifndef _ARP_H_
#define _ARP_H_

#include "mbuf.h"

int arp_init();
int arp_rx(struct mbuf* buf);
int arp_tx_reply(struct mbuf* buf);
int arp_tx_request(struct mbuf* buf);

#endif
