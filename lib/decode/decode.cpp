#include "packet.h"

struct packet decode(uint8_t* packet_bytes){ 


    struct packet decoding_packet;
    uint8_t packet_type = packet_bytes[0];
    uint8_t packet_ID = packet_bytes[1<<2];
    uint8_t message_source = packet_bytes[3];
    uint8_t packet_dest = packet_bytes[4];
    uint8_t message_size = packet_bytes[5];
    decoding_packet.size = message_size;

    uint8_t* ptr_decoded_bytes = decoded_bytes;
    
    while(*ptr_message++){
        *ptr_decoded_bytes = (char)*ptr_message;
        ptr_decoded_bytes++;
    }

    decoding_packet.decoded_bytes = decoded_bytes;

    return decoding_packet;
}