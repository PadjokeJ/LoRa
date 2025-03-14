#ifndef PACKET_H
#define PACKET_H

struct packet{
    uint8_t type;
    uint16_t identifier;
    uint8_t source;
    uint8_t destination;
    char* message;
    uint8_t* encoded_bytes;
}
