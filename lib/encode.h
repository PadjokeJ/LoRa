#ifdef __cplusplus
extern "C"
{
#endif

#ifndef ENCODE_H
#define ENCODE_H

uint8_t* encode(uint4_t packet_type, uint8_t source_address, uint8_t destination_address, char* message);

#ifdef __cplusplus
}
#endif