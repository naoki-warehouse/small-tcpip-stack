#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "unix_sock.h"

static int unix_sock_open(){
    int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(fd == -1){
        fprintf(stderr, "Failed to create unix domain socket\n");
        return -1;
    }

    return fd;
}

int unix_sock_open_server(const char* sock_name){
    int fd = unix_sock_open();

    struct sockaddr_un sun;
    memset(&sun, 0, sizeof(sun));
    sun.sun_family = AF_LOCAL;
    strcpy(sun.sun_path, sock_name);

    int ret = bind(fd, (const struct sockaddr *)&sun, sizeof(sun));
    if(ret == -1){
        fprintf(stderr, "Failed to bind unix domain socket %s\n", sock_name);
        return -1;
    }
    
    ret = listen(fd, 10);
    if(ret == -1){
        fprintf(stderr, "Failed to listen unix domain socket %s\n", sock_name);
        close(fd);
        return -1;
    }

    return fd;
}

int unix_sock_open_client(const char* sock_name){
    int fd = unix_sock_open(sock_name);

    struct sockaddr_un sun;
    memset(&sun, 0, sizeof(sun));
    sun.sun_family = AF_LOCAL;
    strcpy(sun.sun_path, sock_name);

    int ret = connect(fd, (const struct sockaddr *)&sun, sizeof(sun));
    if(ret == -1){
        fprintf(stderr, "Failed to connect unix domain socket %s\n", sock_name);
        close(fd);
        return -1;
    }

    return fd;
}
