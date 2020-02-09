#ifndef _SOCKET_MANAGER_H_
#define _SOCKET_MANAGER_H_

int socket_manager_init();
int socket_manager_listen_thread();
int socket_manager_rx_thread();
int socket_manager_tx_thread();
int socket_manager_create_raw_socket();
int socket_manager_add_raw_packet();
int socket_manager_get_raw_packet();

#endif
