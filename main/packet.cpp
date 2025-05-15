/**
 * Code for packet structs to facilitate data management in main loop
 *
 * Code redacted and commented by padjokej.dev
**/

#include <stdint.h>
#include "packet.h"

struct packet to_packet_struct(uint8_t packet_type, uint16_t identifier, uint8_t source_address, uint8_t destination_address, char* message) { // function that returns a packet structure from certain informations
    struct packet packet_to_return; // create an uninitalised packet structure
    packet_to_return.type = packet_type; // retrieve the packet type from function arguments
    packet_to_return.identifier = identifier; // retrieve the packet identifier from function arguments

    packet_to_return.source = source_address; // retrieve the packet source address from function arguments
    packet_to_return.destination = destination_address; // retrieve the packet destination address from function arguments

    packet_to_return.message = message; // retrieve the packet message contents from function arguments
    
    return packet_to_return; // returns the packet structure
}

struct packet create_recieve_packet(uint16_t original_identifier, uint8_t original_source, uint8_t original_dest){ // unused function --> delete later
    struct packet packet_to_return;

    /* UNUSED
    packet_to_return.type = ~1;
    packet_to_return.identifier = original_identifier;

    packet_to_return.source = original_dest;
    packet_to_return.destination = original_source;

    packet_to_return.message = "\0";
    */

    return packet_to_return;
}