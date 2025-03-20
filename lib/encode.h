#ifndef ENCODE_H
#define ENCODE_H

#include "packet.h"

struct packet encode(uint8_t packet_type, uint8_t source_address, uint8_t destination_address, char* message);

#endif