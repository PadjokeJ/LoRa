/*
*   Run commands : 
*   g++ .\lib\encode\encode_test.cpp .\lib\encode\encode.cpp .\lib\decode\decode.cpp .\lib\packet\packet.cpp -o app.exe
*   .\app.exe
*/


#include "../encode/encode.h"
#include "../packet/packet.h"
#include "../decode/decode.h"

#include <stdio.h>
#include <stdint.h>

int main(){
    uint8_t packet_type = 0;
    uint8_t source = 0;
    uint8_t dest = 1;

    char message[] = "Hello World!";

    struct packet pack;

    pack = encode_message_to_send(source, dest, message);

    printf("ID \t:\t%d\n", pack.identifier);
    printf("Type \t:\t%d\n", pack.type);
    printf("source \t:\t%d\n", pack.source);
    printf("dest \t:\t%d\n", pack.destination);
    printf("size \t:\t%d\n", pack.size);
    printf("message\t:\t%s\n", pack.message);

    putchar('\n');

    uint8_t* bytess = pack.encoded_bytes;
    while(*bytess++){
        printf("%d, ", *bytess);
    }
    putchar('\n');

    struct packet pack2 = decode(pack.encoded_bytes);

    printf("ID \t:\t%d\n", pack2.identifier);
    printf("Type \t:\t%d\n", pack2.type);
    printf("source \t:\t%d\n", pack2.source);
    printf("dest \t:\t%d\n", pack2.destination);
    printf("size \t:\t%d\n", pack2.size);
    printf("message\t:\t%s\n", pack2.message);

    return 0;
}