#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "arp_table.h"
#include "arp.h"

#define ARP_TABLE_REFRESH_SEC 10
struct arp_list {
    struct arp_list *prev;
    struct arp_list *next;
    struct arp_entry entry;
};

static struct arp_list *arp_table = NULL;
static pthread_t arp_table_thread_t;

int arp_table_init(){
    int ret = pthread_create(&arp_table_thread_t, NULL, arp_table_thread, NULL);
    if(ret != 0){
        fprintf(stderr, "Failed to create arp_table_thread\n");
        return -1;
    }
    return 0;
}

void* arp_table_thread(void *arg){
    while(1){
        time_t time_now = time(NULL);
        struct arp_list *entry = arp_table;
        while(entry != NULL){
            if(time_now - entry->entry.update_time > ARP_TABLE_REFRESH_SEC){
                fprintf(stdout, "ARP Table Entry Expired\n");
            }
            entry = entry->next;
        }
        sleep(1);
    }
}

struct arp_entry* arp_table_add(uint8_t *hw, uint8_t *ip){
    struct arp_entry *entry;
    entry = arp_table_get_by_hw(hw);
    if(entry != NULL){
        memcpy(entry->ip, ip, 4);
        entry->update_time = time(NULL);
        return entry;
    }

    entry = arp_table_get_by_ip(ip);
    if(entry != NULL){
        memcpy(entry->hw, hw, 6);
        entry->update_time = time(NULL);
        return entry;
    }

    struct arp_list *table_tail = arp_table;
    struct arp_list *entry_prev = NULL;

    if(table_tail != NULL){
        entry_prev = arp_table;
        while(table_tail->next != NULL){
            entry_prev = table_tail;
            table_tail = table_tail->next;
        }
        table_tail->next = malloc(sizeof(struct arp_list));
        table_tail = table_tail->next;
    }else{
        arp_table = malloc(sizeof(struct arp_list));
        table_tail = arp_table;
    }

    if(table_tail == NULL){
        fprintf(stderr, "Failed to allocate arp_table entry\n");
        return NULL;
    }

    memcpy(table_tail->entry.hw, hw, 6);
    memcpy(table_tail->entry.ip, ip, 6);
    table_tail->entry.update_time = time(NULL);
    table_tail->prev = entry_prev;
    table_tail->next = NULL;
    table_tail->entry.list = table_tail;

    return &table_tail->entry;
}

int arp_table_delete(struct arp_entry *arp_entry){
    struct arp_entry *entry = arp_table_get_by_hw(arp_entry->hw);
    if(entry == NULL){
        entry = arp_table_get_by_ip(arp_entry->ip);
    }
    if(entry == NULL){
        return 0;
    }
    struct arp_list *list = entry->list;
    if(list->prev == NULL){
        arp_table = list->next;
    }else{
        list->prev->next = list->next;
    }
    if(list->next != NULL){
        list->next->prev = list->prev;
    }
    free(list);
}

void arp_table_list(){
    struct arp_list *entry_now = arp_table;
    printf("|                   ARP Table                 |\n");
    printf("|   MAC Address   |   IP Address  |Update Time|\n");
    while(entry_now != NULL){
        uint8_t *hw = entry_now->entry.hw;
        uint8_t *ip = entry_now->entry.ip;
        printf("|%02X:%02X:%02X:%02X:%02X:%02X|%03d.%03d.%03d.%03d|%011d|\n",
                hw[0], hw[1], hw[2], hw[3], hw[4], hw[5],
                ip[0], ip[1], ip[2], ip[3], entry_now->entry.update_time);
        entry_now = entry_now->next;
    }
}

struct arp_entry *arp_table_get_by_hw(uint8_t *hw){
    struct arp_list *entry_now = arp_table;
    while(entry_now != NULL){
        if(memcmp(entry_now->entry.hw, hw, 6) == 0) return &entry_now->entry;
        entry_now = entry_now->next;
    }
    return NULL;
}

struct arp_entry *arp_table_get_by_ip(uint8_t *ip){
    struct arp_list *entry_now = arp_table;
    while(entry_now != NULL){
        if(memcmp(entry_now->entry.ip, ip, 4) == 0) return &entry_now->entry;
        entry_now = entry_now->next;
    }
    return NULL;
}
