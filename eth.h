#ifndef _ETH_H_
#define _ETH_H_

#include "mbuf.h"

int eth_rx(struct mbuf *buf);
int eth_tx();

#endif
