#ifndef _SOCKET_MANAGER_H_
#define _SOCKET_MANAGER_H_

#include <stdint.h>

int socket_manager_init();
void* socket_manager_listen_thread(void *arg);
void* socket_manager_rx_thread(void *arg);
int socket_manager_tx_thread();

struct socket_entry* socket_manager_create_raw_socket(int fd);
int socket_manager_delete_raw_socket(int fd);
struct socket_entry* socket_manager_get_raw_socket(int fd);
void socket_manager_list_raw_socket();


int socket_manager_add_raw_packet(const uint8_t *buf, int len);
int socket_manager_get_raw_packet();

#endif
