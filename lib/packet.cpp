
#include <stdint.h>

struct packet to_packet_struct(uint8_t packet_type, uint16_t identifier, uint8_t source_address, uint8_t destination_address, char* message){
    struct packet packet_to_return;
    packet_to_return.type = packet_type;
    packet_to_return.identifier = identifier;
    packet_to_return.source = source_address;
    packet_to_return.destination = destination_address;
    packet_to_return.message = message;
    
    return packet_to_return;
}