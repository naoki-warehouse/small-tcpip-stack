#ifndef _NETDEV_H_
#define _NETDEV_H_

#include <stdint.h>
#include "mbuf.h"

struct netdev_info {
    uint8_t hw_addr[6];
    uint8_t ip_addr[4];
    int mtu;

    struct netdev_info *pre;
    struct netdev_info *next;
};

int netdev_init(const char* dev_name);
void* netdev_rx_thread(void *arg);
int netdev_tx(void *buf);

struct netdev_info* netdev_add(uint8_t* hw_addr, uint8_t* ip_addr, int mtu);

struct netdev_info* netdev_get_by_ip(uint8_t* ip_addr);
struct netdev_info* netdev_get_by_hw(uint8_t* hw_addr);

void netdev_list();
void netdev_print(struct netdev_info* netdev);
#endif
