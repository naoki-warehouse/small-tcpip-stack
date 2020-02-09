#ifndef _UNIX_SOCK_H_
#define _UNIX_SOCK_H_

int unix_sock_open_server(const char* sock_name);
int unix_sock_open_client(const char* socke_name);
#endif
