#include <time.h>
#include <stdlib.h>

#include <stdint.h>


#include "packet.h"

uint8_t size_of_message(char* message){
    uint8_t size;
    while(*message++)
        size++;

    return size;
}

struct packet encode(uint8_t packet_type, uint8_t source_address, uint8_t destination_address, char* message){ 
    srand(time(NULL)); // set the seed of the random number generator
    int random_number = rand(); // get a random number

    uint16_t identifier = (uint16_t)random_number; //convert the random number to a 16 bit type

    struct packet encoding_packet = to_packet_struct(packet_type, identifier, source_address, destination_address, message); // convert the packet info to a struct

    char* ptr_message = encoding_packet.message; // set a pointer to the start of the message array

    uint8_t message_size = size_of_message(message); // get the message size
    encoding_packet.size = message_size; // set the message size inside the packet structure

    uint8_t* encoded_bytes{new uint8_t[message_size + 6]}; // create an uninitialized array with the correct size to include all the information
    uint8_t* ptr_encoded_bytes = encoded_bytes; // set a pointer to the start of the encoded array start
    
    *ptr_encoded_bytes = encoding_packet.type; // copy the packet type byte to the encoded bytes
    ptr_encoded_bytes++; // move the pointer to packet id

    *ptr_encoded_bytes = (uint8_t)(encoding_packet.identifier >> 8); // copy the packet identifier byte (the first eight bits) to the encoded bytes
    ptr_encoded_bytes++; // move the pointer to packet id's second byte
    *ptr_encoded_bytes = (uint8_t)encoding_packet.identifier; // copy the packet identifier byte (the last eight bits) to the encoded bytes
    ptr_encoded_bytes++; // move the pointer to source
    
    *ptr_encoded_bytes = encoding_packet.source; // copy the packet type byte to the encoded bytes
    ptr_encoded_bytes++; // move the pointer to dest

    *ptr_encoded_bytes = encoding_packet.destination; // copy the packet type byte to the encoded bytes
    ptr_encoded_bytes++; // move the pointer to size

    *ptr_encoded_bytes = encoding_packet.size; // copy the packet type byte to the encoded bytes
    ptr_encoded_bytes++; // move the pointer to message start


    while(*ptr_message++){
        *ptr_encoded_bytes = (uint8_t)*ptr_message; // transform char to 8bit
        ptr_encoded_bytes++;
    }


    encoding_packet.encoded_bytes = encoded_bytes; //set the message bytes inside of the packet structure

    return encoding_packet;
}