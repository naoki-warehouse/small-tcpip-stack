#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "netdev.h"
#include "dev_tap.h"
#include "socket_manager.h"
#include "config.h"
#include "utils.h"
#include "mbuf.h"
#include "eth.h"

struct netdev_info *netdev_head = NULL;

static pthread_t rx_thread;

int netdev_init(const char* dev_name){
    int tap_fd = dev_tap_open(dev_name);
    if(tap_fd == -1){
        fprintf(stderr, "Failed to init netdev\n");
        return -1;
    }
    pthread_create(&rx_thread, NULL, netdev_rx_thread, &tap_fd);

    run_cmd("ip a add 192.168.100.100/24 dev %s", dev_name);
    run_cmd("ip link set dev %s up", dev_name);

    return 0;
}

void *netdev_rx_thread(void *arg){
    int fd = *((int *)arg);
    fprintf(stdout, "Starting to recv netdev...\n");
    while(1){
        struct mbuf *buf = mbuf_alloc(CONFIG_SOCKET_MANAGER_BUF_SIZE);
        if(buf == NULL){
            fprintf(stderr, "Failed to allocate mbuf\n");
            return NULL;
        }
        int size = dev_tap_read(fd, buf->data, buf->dlen);
        buf->fd = fd;
        buf->plen = size;
        //printf("Recv netdev size:%d\n", size);
        socket_manager_add_raw_packet(buf->data, size);
        eth_rx(buf);
    }
}

int netdev_tx(void *buf){
    struct mbuf *mbuf = (struct mbuf *)buf;
    dev_tap_write(mbuf->fd, mbuf->data, mbuf->plen);
    mbuf_free_all(mbuf);
}

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

struct netdev_info* netdev_add_s(const char* hw_addr, const char* ip_addr, int mtu){
    uint8_t hw[6];
    uint8_t ip[4];
    sscanf(hw_addr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", 
            &hw[0], &hw[1], &hw[2], &hw[3], &hw[4], &hw[5]);
    uint32_t ip_t = inet_addr(ip_addr);
    ip[0] = ip_t&0xFF;
    ip[1] = (ip_t>>8)&0xFF;
    ip[2] = (ip_t>>16)&0xFF;
    ip[3] = (ip_t>>24)&0xFF;
    return netdev_add(hw, ip, mtu);
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
    return NULL;
}

void netdev_print(struct netdev_info *netdev){
    uint8_t *hw = netdev->hw_addr;
    uint8_t *ip = netdev->ip_addr;
    printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X IP:%03d.%03d.%03d.%03d\n",
            hw[0], hw[1], hw[2], hw[3], hw[4], hw[5],
            ip[0], ip[1], ip[2], ip[3]);
}
