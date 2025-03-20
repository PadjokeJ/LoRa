#include "packet.h"
#include <time.h>
#include <stdlib.h>

#include <stdint.h>


uint8_t size_of_message(char* message){
    uint8_t size;
    while(*message++)
        size++;

    return size;
}

struct packet encode(uint8_t packet_type, uint8_t source_address, uint8_t destination_address, char* message){ 
    srand(time(NULL));
    int random_number = rand();

    uint16_t identifier = (uint16_t)random_number;

    struct packet encoding_packet = to_packet_struct(packet_type, identifier, source_address, destination_address, message);

    char* ptr_message = encoding_packet.message;
    uint8_t message_size = size_of_message(message);
    encoding_packet.size = message_size;
    uint8_t encoded_bytes[message_size];
    uint8_t* ptr_encoded_bytes = encoded_bytes;
    
    while(*ptr_message++){
        *ptr_encoded_bytes = (uint8_t)*ptr_message;
        ptr_encoded_bytes++;
    }

    encoding_packet.encoded_bytes = encoded_bytes;

    return encoding_packet;
}