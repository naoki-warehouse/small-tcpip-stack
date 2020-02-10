#include <sys/socket.h>
#include <unistd.h>
#include "my_sock.h"


int main(int argc, char *argv[]){
    int fd = my_sock_socket(AF_INET, SOCK_RAW, 0);
    while(1){
        sleep(1);
    }

    return 0;
}
