#include "encode.h"
#include "packet.h"

#include <stdint.h>

int main(){
    uint8_t packet_type = 0;
    uint8_t source = 0;
    uint8_t dest = 1;

    char* message = "Hello World!";

    struct packet pack = encode(packet_type, source, dest, message);

    return 0;
}