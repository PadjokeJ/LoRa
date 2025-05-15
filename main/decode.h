#ifndef DECODE_H // if we haven't declared this header yet, then do : (avoids duplicate header creation)
#define DECODE_H // declare this header

#include "packet.h" // include packet structure

//declare function to decode a packet structure into a message, that we copy into the message buffer
struct packet decode(uint8_t* packet_bytes, char* message_buffer);

//declatre a function to print out what type of packet we are dealing with
int get_packet_type(struct packet pack);

#endif // exit out of the header declaration