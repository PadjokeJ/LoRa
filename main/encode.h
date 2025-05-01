#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>
#include "packet.h"

uint8_t size_of_message(char* message);

struct packet encode(uint8_t packet_type, uint16_t identifier, uint8_t source_address, uint8_t destination_address, char* message);

struct packet encode_message_reciept(struct packet recieved_packet);

struct packet encode_message_to_send(uint8_t source, uint8_t dest, char* message);

#endif