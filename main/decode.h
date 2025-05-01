#ifndef DECODE_H
#define DECODE_H

#include "packet.h"

struct packet decode(uint8_t* packet_bytes);

int get_packet_type(struct packet pack);

#endif