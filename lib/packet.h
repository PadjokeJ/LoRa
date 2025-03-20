#ifndef PACKET_H
#define PACKET_H

struct packet{
    uint8_t type;
    uint16_t identifier;
    uint8_t source;
    uint8_t destination;
    uint8_t size;
    char* message;
    uint8_t* encoded_bytes;
}

struct packet to_packet_struct(uint8_t packet_type, uint16_t identifier, uint8_t source_address, uint8_t destination_address, char* message);
#endif