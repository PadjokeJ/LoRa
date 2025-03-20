#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>
#include "../packet/packet.h"

uint8_t size_of_message(char* message);

struct packet encode(uint8_t packet_type, uint8_t source_address, uint8_t destination_address, char* message);

#endif