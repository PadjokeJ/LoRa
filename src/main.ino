/*
*   Compile command:
*       g++ .\src\main.cpp .\lib\encode\encode.cpp .\lib\decode\decode.cpp .\lib\packet\packet.cpp .\lib\memory\memory.cpp .\lib\process\process.cpp .\lib\recieve\recieve.cpp .\lib\send\send.cpp .\lib\serial\serial.cpp -o app.exe
*   Run command:
*       .\app.exe
*/

#include <Arduino.h>
#include "../lib/encode/encode.h"
#include "../lib/decode/decode.h"
#include "../lib/packet/packet.h"
#include "../lib/memory/memory.h"
#include "../lib/process/process.h"
#include "../lib/recieve/recieve.h"
#include "../lib/send/send.h"
#include "../lib/serial/serial.h"

#include "config.h"
#include <RH_RF95.h>

#define LORA_SS 10     
#define LORA_FREQ 868

uint8_t messageBytesBuffer[MAX_BYTES_LEN] = {0};
char messageBuffer[MAX_MESSAGE_LEN] = {0};

bool should_show_message = 0;

Lorainit lora(LORA_SS, LORA_FREQ);
Receive receiver;
Send sender;

void resetBuffers(){ 
    for (int i = 0; i < MAX_BYTES_LEN; i++)
        messageBytesBuffer[i] = 0;
    for (int i = 0; i < MAX_MESSAGE_LEN; i++)
        messageBuffer[i] = 0;
}

uint8_t decodeAndAnalyseMessage(uint8_t* incomingBytes, char* bufferToModify){
    struct packet incomingPacket;
    // decode the message
    incomingPacket = decode(incomingBytes);

    bool hasSeenThisMessage = comparaison(incomingPacket.identifier, comparison_list, COMPARISON_LIST_SIZE);

    if (hasSeenThisMessage){
        return MESSAGE_SEEN_CODE;
    }
    if(my_address != incomingPacket.destination){
        return MESSAGE_TO_FORWARD_CODE;
    }
    should_show_message = 1;
    bufferToModify = incomingPacket.message;

    return MESSAGE_TO_ME_CODE;
    
}

void setup() {
    lora.init();
}

void loop() {
    // <- fetch message into bytes buffer

    //ADD CODE HERE

    // <- decode message, and see what to do with it

    uint8_t result = decodeAndAnalyseMessage(messageBytesBuffer, messageBuffer);

    // <- decide what to do
    if (result == MESSAGE_SEEN_CODE)
        ; //do nothing? (include read reciepts, to see if message has gone further)
    if (result == MESSAGE_TO_FORWARD_CODE)
        ; //send message again
    if (result == MESSAGE_TO_ME_CODE)
        ; //display message on serial
}
