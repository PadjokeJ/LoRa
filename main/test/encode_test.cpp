/*
*   Run commands : 
*   g++ .\lib\encode\encode_test.cpp .\lib\encode\encode.cpp .\lib\decode\decode.cpp .\lib\packet\packet.cpp -o app.exe
*   .\app.exe
*/


#include "../encode.h" // include code for encoding
#include "../packet.h" // include code for packet structure and management
#include "../decode.h" // include code for decoding

#include <stdio.h>
#include <stdint.h>

char message_buffer[200]; // create a buffer to decode the message into

int main(){
    uint8_t source = 60; // set the user's address
    uint8_t dest = 53; // set the destination's address

    char message[] = "Hello World!"; // create a string with the message "Hello World!"

    struct packet pack; // create an empty packet

    pack = encode_message_to_send(source, dest, message); // call the encode function

    // print out the encoded packet and its proprieties
    printf("ID \t:\t%d\n", pack.identifier);
    printf("Type \t:\t%d\n", pack.type);
    printf("source \t:\t%d\n", pack.source);
    printf("dest \t:\t%d\n", pack.destination);
    printf("size \t:\t%d\n", pack.size);
    printf("message\t:\t%s\n", pack.message);

    putchar('\n'); // line break in the console --> air it out

    // debug the encoded bytes
    uint8_t* encode_bytes = pack.encoded_bytes; 
    while(*encode_bytes++){
        printf("%d, ", *encode_bytes);
    }
    putchar('\n'); // line break in the console --> air it out

    struct packet pack2 = decode(pack.encoded_bytes, message_buffer); // call the decoded function, set it to a new packet structure

    // print out the decoded packet and its proprieties
    printf("ID \t:\t%d\n", pack2.identifier);
    printf("Type \t:\t%d\n", pack2.type);
    printf("source \t:\t%d\n", pack2.source);
    printf("dest \t:\t%d\n", pack2.destination);
    printf("size \t:\t%d\n", pack2.size);
    printf("message\t:\t%s\n", pack2.message);

    printf("buffer \t:\t%s\n", message_buffer); // print out the message we decoded into the buffer

    // if the encode and decode code is correct, we should have the same proprieties and message when we print them out

    return 0;
}