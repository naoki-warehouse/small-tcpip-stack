#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "dev_tap.h"

char dev_tap_name[IFNAMSIZ] = {0};
int dev_tap_open(const char *tap_name){
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));

    int fd = open("/dev/net/tun", O_RDWR);
    if(fd == -1){
        fprintf(stderr, "Failed to open /dev/net/tun\n");
        return -1;
    }

    strncpy(dev_tap_name, tap_name, IFNAMSIZ);

    ifr.ifr_flags = IFF_TAP|IFF_NO_PI;
    strncpy(ifr.ifr_name, dev_tap_name, IFNAMSIZ);
    int ret = ioctl(fd, TUNSETIFF, &ifr);
    if(ret == -1){
        fprintf(stderr, "Failed to configure TAP device\n");
        close(fd);
        return -1;
    }

    fprintf(stdout, "Open TAP device %s\n", ifr.ifr_name);
    return fd;
}

int dev_tap_read(int fd, uint8_t *buf, int buf_len){
    return read(fd, buf, buf_len);
}

int dev_tap_write(int fd, uint8_t *buf, int buf_len){
    return write(fd, buf, buf_len);
}

int dev_tap_close(int fd){
    close(fd);
}
