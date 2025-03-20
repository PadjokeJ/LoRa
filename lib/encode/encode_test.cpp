#include "encode.h"
#include "packet.h"

#include <stdint.h>

int main(){
    uint8_t packet_type = 0;
    uint8_t source = 0;
    uint8_t dest = 1;

    char message[] = "Hello World!";

    struct packet pack;

    encode(packet_type, source, dest, message);

    printf("ID \t:\t%d\n", pack.identifier);
    printf("Type \t:\t%d\n", pack.type);
    printf("source \t:\t%d\n", pack.source);
    printf("dest \t:\t%d\n", pack.destination);
    printf("size \t:\t%d\n", pack.size);

    return 0;
}