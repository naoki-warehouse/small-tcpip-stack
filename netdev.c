#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "netdev.h"
struct netdev_info *netdev_head = NULL;

struct netdev_info *netdev_add(uint8_t *hw_addr, uint8_t* ip_addr, int mtu){
    struct netdev_info *netdev_tail = netdev_head;
    struct netdev_info *netdev_pre = NULL;

    if(netdev_tail != NULL){
        netdev_pre = netdev_head;
        while(netdev_tail->next != NULL){
            netdev_pre = netdev_tail;
            netdev_tail = netdev_tail->next;
        }
        netdev_tail->next = malloc(sizeof(struct netdev_info));
        netdev_tail = netdev_tail->next;
    }else{
        netdev_head = malloc(sizeof(struct netdev_info));
        netdev_tail = netdev_head;
    }

    if(netdev_tail == NULL){
        fprintf(stderr, "Failed to allocate netdev_info\n");
        return NULL;
    }

    memcpy(netdev_tail->hw_addr, hw_addr, 6);
    memcpy(netdev_tail->ip_addr, ip_addr, 4);
    netdev_tail->mtu = mtu;
    netdev_tail->pre = netdev_pre;
    netdev_tail->next = NULL;

    return netdev_tail;
}

void netdev_list(){
    struct netdev_info *netdev_now = netdev_head;
    printf("|   MAC Address   |   IP Address  |\n");
    while(netdev_now != NULL){
        uint8_t *hw = netdev_now->hw_addr;
        uint8_t *ip = netdev_now->ip_addr;
        printf("|%02X:%02X:%02X:%02X:%02X:%02X|%03d.%03d.%03d.%03d|\n",
                hw[0], hw[1], hw[2], hw[3], hw[4], hw[5],
                ip[0], ip[1], ip[2], ip[3]);
        netdev_now = netdev_now->next;
    }
}

struct netdev_info* netdev_get_by_ip(uint8_t* ip_addr){
    struct netdev_info *netdev_now = netdev_head;
    while(netdev_now != NULL){
        if(memcmp(netdev_now->ip_addr, ip_addr, 4) == 0) return netdev_now;
        netdev_now = netdev_now->next;
    }
    return NULL;
}

struct netdev_info* netdev_get_by_hw(uint8_t* hw_addr){
    struct netdev_info *netdev_now = netdev_head;
    while(netdev_now != NULL){
        if(memcmp(netdev_now->hw_addr, hw_addr, 6) == 0) return netdev_now;
        netdev_now = netdev_now->next;
    }
}

void netdev_print(struct netdev_info *netdev){
    uint8_t *hw = netdev->hw_addr;
    uint8_t *ip = netdev->ip_addr;
    printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X IP:%03d.%03d.%03d.%03d\n",
            hw[0], hw[1], hw[2], hw[3], hw[4], hw[5],
            ip[0], ip[1], ip[2], ip[3]);
}
