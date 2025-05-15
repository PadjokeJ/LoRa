#ifndef ENCODE_H // if we haven't declared this header yet, then do : (avoids duplicate header creation)
#define ENCODE_H // declare this header

#include <stdint.h> // include standard integer library, for uint8_t and uint16_t
#include "packet.h" // include packet structure and function to create packets quickly

// equivalent of strlen, so we don't have to include heavy string.h library, unused, since arduino comes with strlen function
uint8_t size_of_message(char* message);

// declaration of encode function, that takes in a type, id, message source, destination, message to encode, as well as the reference to the buffer to write it into
struct packet encode(uint8_t packet_type, uint16_t identifier, uint8_t source_address, uint8_t destination_address, char* message, uint8_t* buffer);

// declaration of encoding a read message receipt, unused
struct packet encode_message_reciept(struct packet recieved_packet, uint8_t* buffer);

// declaration to quickly create a packet with just a source, message, destination and buffer to write into, automatically generates the message id
struct packet encode_message_to_send(uint8_t source, uint8_t dest, char* message, uint8_t* buffer);

#endif