#include "unix_sock.h"
#include <sys/socket.h>
#include <string.h>

const char* sock = "/tmp/sts/sock";
int main(int argc, char *argv[]){
    int fd = unix_sock_open_client(sock);

    const char* test_str = "Hello World";
    send(fd, test_str, strlen(test_str), 0);

    return 0;
}
