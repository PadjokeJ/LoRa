#include <stdint.h>
#include "packet.h"

struct packet to_packet_struct(uint8_t packet_type, uint16_t identifier, uint8_t source_address, uint8_t destination_address, char* message) {
    struct packet packet_to_return;
    packet_to_return.type = packet_type;
    packet_to_return.identifier = identifier;

    packet_to_return.source = source_address;
    packet_to_return.destination = destination_address;

    packet_to_return.message = message;
    
    return packet_to_return;
}

struct packet create_recieve_packet(uint16_t original_identifier, uint8_t original_source, uint8_t original_dest){
    struct packet packet_to_return;

    packet_to_return.type = 2;
    packet_to_return.identifier = original_identifier;

    packet_to_return.source = original_dest;
    packet_to_return.destination = original_source;

    packet_to_return.message = "\0";

    return packet_to_return;
}