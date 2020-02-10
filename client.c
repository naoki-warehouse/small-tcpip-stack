#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "my_sock.h"


int main(int argc, char *argv[]){
    int fd = my_sock_socket(AF_INET, SOCK_RAW, 0);
    uint8_t buf[1500];
    while(1){
        memset(buf, 0, sizeof(buf));
        int size = my_sock_recv(fd, buf, sizeof(buf), 0);
        if(size < 1){
            fprintf(stderr, "Failed to recv\n");
            continue;
        }
        for(int i=0;i<size;i++){
            printf("%02X ", buf[i]);
        }
        printf("%s\n", buf);
    }

    return 0;
}
