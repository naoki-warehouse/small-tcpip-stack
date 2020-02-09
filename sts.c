#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

#include "file.h"
#include "unix_sock.h"
#include "dev_tap.h"
#include "utils.h"

const char* sts_dir = "/tmp/sts/";
const char* sock = "/tmp/sts/sock";

int main(int argc, char* argv[]){

    if(!file_exist(sts_dir)){
        fprintf(stdout, "STS dir %s does not exist. Creating...\n", sts_dir);
        if(!file_create_dir(sts_dir)){
            fprintf(stderr, "Failed to create STS dir %s\n", sts_dir);
            return -1;
        }
        fprintf(stdout, "Create STS dir %s\n", sts_dir);
    }else{
        fprintf(stdout, "STS dir %s exists\n", sts_dir);
    }

    if(file_exist(sock)){
        fprintf(stdout, "Socket %s exists. Deleting...\n", sock);
        if(file_delete_file(sock)){
            fprintf(stdout, "Delete Socket %s\n", sock);
        }else{
            fprintf(stderr, "Fialed to delete Socket %s\n", sock);
            return -1;
        }
    }

    int fd = unix_sock_open_server(sock);

    int tap_fd = dev_tap_open("test");
    /*
    struct sockaddr_un sun;
    int sock_len = 0;
    int fd_accept = accept(fd, (struct sockaddr *)&sun, &sock_len);
    if(fd_accept == -1){
        fprintf(stderr, "Faild to accept unix domain socket %s\n", sock);
        close(fd);
        return -1;
    }

    uint8_t buf[1500];
    memset(buf, 0, sizeof(buf));

    recv(fd_accept, buf, sizeof(buf), 0);
    printf("RECV:%s\n", buf);
    */
    run_cmd("ip a add 192.168.100.100/24 dev test");

    while(1){
        sleep(1);
    }

    if(file_delete_file(sock)){
        fprintf(stdout, "Delete Socket %s\n", sock);
    }else{
        fprintf(stderr, "Fialed to delete Socket %s\n", sock);
        return -1;
    }
    return 0;
}
