#include <stdio.h>
#include <stdint.h>

#include "decode.h"
#include "../packet/packet.h"

struct packet decode(uint8_t* packet_bytes){
    struct packet decoding_packet; // declare a packet structure
    decoding_packet.encoded_bytes = packet_bytes; // take the parameter of the function decode

    uint8_t packet_type = packet_bytes[0]; // get the first encoded byte as the packet type
    decoding_packet.type = packet_type; // set the packet type inside the packet structure
    
    uint16_t packet_ID = (((uint16_t)packet_bytes[1] << 8) & 0xFF00) + ((uint16_t)packet_bytes[2] & 0x00FF); // get the second byte, move it by a byte and remove residues with a bit operator. get the third byte, add it to the variable

    decoding_packet.identifier = packet_ID; // set the packet ID inside the packet structure

    uint8_t message_source = packet_bytes[3]; // get the fourth encoded byte as the message source
    decoding_packet.source = message_source; // set the message source inside the packet structure

    uint8_t packet_dest = packet_bytes[4]; // get the fifth encoded byte as the message destination
    decoding_packet.destination = packet_dest; // set the message destination inside the packet structure

    uint8_t message_size = packet_bytes[5]; // get the sixth encoded byte as the message size
    decoding_packet.size = message_size; // set the message size inside the packet structure

    uint8_t* ptr_packet_bytes = &packet_bytes[6]; // put a pointer at the begining of the encoded message
    
    char* ptr_decoded_bytes{new char[message_size]}; // create an array that have the size of the message
    char* decoded_bytes_array_address = ptr_decoded_bytes; // cpoy the adresse of the begining of the array

    while(*ptr_packet_bytes){ // loop until it is at the end of the array
        *ptr_decoded_bytes = (char)*ptr_packet_bytes; // take the encoded byte and turn it into a character
        ptr_decoded_bytes++; // go to the next byte to write to
        ptr_packet_bytes++; // go to the next byte to decode
    }

    decoding_packet.message = decoded_bytes_array_address; // put the decoded message in the packet structure
    return decoding_packet;
}

int get_packet_type(struct packet pack){
    uint8_t packet_type = pack.type; // get the packet type from the packet structure
    
    printf("\nPacket type: %d is a ", packet_type);

    switch (packet_type){ // print what the packet does in function of the packet type
        case 0:
            printf("network discovery packet\n");
            break;
        case 1:
            printf("message packet\n");
            break;
        case ~1:
            printf("read receipt packet\n");
            break;
        default:
            printf("unrecognized packet\n");
    }

    return packet_type;
}