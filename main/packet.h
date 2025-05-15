#ifndef PACKET_H // if we haven't declared this header yet, then do : (avoids duplicate header creation)
#define PACKET_H // declare this header

#include <stdint.h> // include int header, for uint8_t

// declare the packet structure and its contents --> type, id, source, dest, message size, message, and a buffer to encode the bytes to
struct packet{
    uint8_t type;
    uint16_t identifier;
    uint8_t source;
    uint8_t destination;
    uint8_t size;
    char* message;
    uint8_t* encoded_bytes;
};

// function to create a packet structure quickly from a few important informations
struct packet to_packet_struct(uint8_t packet_type, uint16_t identifier, uint8_t source_address, uint8_t destination_address, char* message);

// unused function to create a packet that would say we have received a message with this identifier
struct packet create_recieve_packet(uint16_t identifier, uint8_t source, uint8_t dest);

#endif