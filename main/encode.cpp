/**
 * Code to encode strings into uint8_t buffers, alongside useful information
 *
 * Code redacted by padjokej.dev, commented by arthur.delemont
**/

#include <time.h>
#include <stdlib.h>
#include <Arduino.h>

#include <stdint.h>

#include "encode.h"
#include "packet.h"


struct packet encode(uint8_t packet_type, uint16_t identifier, uint8_t source_address, uint8_t destination_address, char* message, uint8_t* buffer){ 
    struct packet encoding_packet = to_packet_struct(packet_type, identifier, source_address, destination_address, message); // convert the packet info to a struct

    char* ptr_message = encoding_packet.message; // set a pointer to the start of the message array

    uint8_t message_size = strlen(message); // get the message size
    encoding_packet.size = message_size; // set the message size inside the packet structure
    
    buffer[0] = encoding_packet.type; // copy the packet type byte to the buffer

    buffer[1] = (uint8_t)((encoding_packet.identifier & 0xFF00) >> 8); // copy the packet identifier byte (the first eight bits) to the buffer
    buffer[2] = (uint8_t)(encoding_packet.identifier & 0x00FF); // copy the packet identifier byte (the last eight bits) to the buffer
    
    buffer[3] = encoding_packet.source; // copy the packet type byte to the buffer

    buffer[4] = encoding_packet.destination; // copy the packet type byte to the buffer

    buffer[5] = encoding_packet.size; // copy the packet type byte to the buffer

    int i = 6;
    while(*ptr_message){
        buffer[i] = (uint8_t)*ptr_message;// transform char to 8bit and put it into the message buffer
        ptr_message++; // increment message pointer, to get to the next char
        i++; // move to next position in message buffer
    }
    buffer[i] = 0; // set the last char of message to string end character


    encoding_packet.encoded_bytes = buffer; //set the message bytes inside of the packet structure

    return encoding_packet; // return the packet we encoded to
}

struct packet encode_message_reciept(struct packet recieved_packet, uint8_t* buffer){
    uint16_t identifier = recieved_packet.identifier; // retrieve the identifier from the packet structure
    uint8_t source = recieved_packet.source; // retrieve the source address from the packet structure
    uint8_t dest = recieved_packet.destination; // retrieve the destination address from the packet structure
    char *message = recieved_packet.message; // retrieve the message array from the packet structure

    uint8_t type = ~ recieved_packet.type; // since typical message is of type 1, invert the bits to get the read type

    return encode(type, identifier, source, dest, message, buffer); // return a packet for this type (read reciept)
}

struct packet encode_message_to_send(uint8_t source, uint8_t dest, char* message, uint8_t* buffer){
    randomSeed(analogRead(0)); // set the seed of the random number generator --> get "noise" from unused arduino port (static electricity)
    int random_number = random(65535); // get a random number, using the arduino's "random seed" from electrical noise

    uint16_t identifier = (uint16_t)random_number; //convert the random number int to a 16 bit type

    return encode(1, identifier, source, dest, message, buffer); // return a packet for this type (message)
}