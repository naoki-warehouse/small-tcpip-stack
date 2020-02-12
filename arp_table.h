#ifndef _ARP_TABLE_H_
#define _ARP_TABLE_H_

#include <stdint.h>
#include <time.h>

struct arp_entry{
    struct arp_list *list;
    uint8_t hw[6];
    uint8_t ip[4];
    time_t update_time;
};

int arp_table_init();
void* arp_table_thread(void *arg);
struct arp_entry* arp_table_add(uint8_t *hw, uint8_t *ip);
int arp_table_delete(struct arp_entry *entry);
struct arp_entry* arp_table_get_by_hw(uint8_t *hw);
struct arp_entry* arp_table_get_by_ip(uint8_t *ip);

void arp_table_list();


#endif
