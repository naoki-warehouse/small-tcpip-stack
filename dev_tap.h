#ifndef _TAP_H_
#define _TAP_H_
#include <stdint.h>

int dev_tap_open(const char *tap_name);
int dev_tap_read(int fd, uint8_t *buf, int buf_len);
int dev_tap_write(int fd, uint8_t *buf, int buf_len);
int dev_tap_close(int fd);

#endif
