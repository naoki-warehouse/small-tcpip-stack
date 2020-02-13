#ifndef _ICMP_H_
#define _ICMP_H_

#include <stdint.h>

#define ICMP_TYPE_ECHO (8)
#define ICMP_TYPE_ECHO_REPLY (0)

struct icmp_pkt {
	uint8_t type;
	uint8_t code;
	uint16_t chksum;
} __attribute__((packed));

struct icmp_echo_pkt {
  uint8_t type;
  uint8_t code;
  uint16_t chksum;
  uint16_t id;
  uint16_t seq_num;
  uint8_t data[56];
} __attribute__((packed));

int icmp_rx(struct mbuf *buf);
int icmp_rx_echo(struct mbuf *buf);
int icmp_rx_echo_reply(struct mbuf *buf);

int icmp_tx(struct mbuf *buf);

uint16_t icmp_chksum(struct mbuf *buf);


#endif
