#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "decode.h"
#include "packet.h"

struct packet decode(uint8_t* packet_bytes, char* message_buffer){
    struct packet decoding_packet; // declare a packet structure
    decoding_packet.encoded_bytes = packet_bytes; // take the parameter of the function decode

    decoding_packet.type = packet_bytes[0]; // get the first encoded byte as the packet type
     
    decoding_packet.identifier = (((uint16_t)packet_bytes[1] << 8) & 0xFF00) + ((uint16_t)packet_bytes[2] & 0x00FF); // get the second byte, move it by a byte and remove residues with a bit operator. get the third byte, add it to the variable

    decoding_packet.source = packet_bytes[3]; // get the fourth encoded byte as the message source

    decoding_packet.destination = packet_bytes[4]; // get the fifth encoded byte as the message destination

    decoding_packet.size = packet_bytes[5]; // get the sixth encoded byte as the message size

    uint8_t* ptr_packet_bytes = &packet_bytes[6]; // put a pointer at the begining of the encoded message
    int i = 0; // start the loop at the first index of the buffer
    
    while(*ptr_packet_bytes && i <= 245){ // loop until it is at the end of the array, either until max message size, or when we hit the end string character
        message_buffer[i] = (char)*ptr_packet_bytes; // take the encoded byte and turn it into a character
        ptr_packet_bytes++; // go to the next byte to decode
        i++; // go to the next byte of message buffer
    }

    decoding_packet.message = message_buffer; // put the decoded message in the packet structure
    
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