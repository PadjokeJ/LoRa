#include "decode.h"
#include "../packet/packet.h"

struct packet decode(uint8_t* packet_bytes){
    struct packet decoding_packet;
    decoding_packet.encoded_bytes = packet_bytes;

    uint8_t packet_type = packet_bytes[0];
    decoding_packet.type = packet_type;
    
    uint16_t packet_ID = (((uint16_t)packet_bytes[1] << 8) & 0xFF00) + ((uint16_t)packet_bytes[2] & 0x00FF);
    decoding_packet.identifier = packet_ID;

    uint8_t message_source = packet_bytes[3];
    decoding_packet.source = message_source;

    uint8_t packet_dest = packet_bytes[4];
    decoding_packet.destination = packet_dest;

    uint8_t message_size = packet_bytes[5];
    decoding_packet.size = message_size;

    uint8_t* ptr_packet_bytes = packet_bytes;
    for (int i = 0; i < 6; i++)
        ptr_packet_bytes++;
    
    
    uint8_t* ptr_decoded_bytes{new uint8_t[message_size]};

    while(*ptr_packet_bytes++){
        *ptr_decoded_bytes = (char)*ptr_packet_bytes;
        ptr_decoded_bytes++;
    }
    return decoding_packet;
}