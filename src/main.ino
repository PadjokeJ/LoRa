/*
*   Compile command:
*       g++ .\src\main.cpp .\lib\encode\encode.cpp .\lib\decode\decode.cpp .\lib\packet\packet.cpp .\lib\memory\memory.cpp .\lib\process\process.cpp .\lib\recieve\recieve.cpp .\lib\send\send.cpp .\lib\serial\serial.cpp -o app.exe
*   Run command:
*       .\app.exe
*/
#define __MAIN_SCRIPT__

#include "config.h"

#include <Arduino.h>
#include "lib/lora_init/lorainit.h"
#include "lib/encode/encode.h"
#include "lib/decode/decode.h"
#include "lib/packet/packet.h"
#include "lib/memory/memory.h"
#include "lib/process/process.h"
#include "lib/recieve/recieve.h"
#include "lib/send/send.h"
#include "lib/serial/serial.h"

#include <RH_RF95.h>

#include <cppQueue.h>

uint8_t messageBytesBuffer[MAX_BYTES_LEN] = {0};
char messageBuffer[MAX_MESSAGE_LEN] = {0};

// Initialisation de la file d'attente avec une capacité maximale de 10 éléments, type uint8_t, mode FIFO
cppQueue myQueue(10, sizeof(uint8_t), FIFO);

bool should_show_message = 0;

Lorainit lora(LORA_SS, LORA_FREQ);

Receive receiver = Receive(lora);
Send sender = Send(lora);

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
    Serial.begin(9600);

    resetBuffers();
}

void loop() {
    resetBuffers();
    // <- fetch message into bytes buffer
    receiver.startReceive(); // switch mode to recieving

    unsigned long time_to_end_recieving = millis() + LISTEN_TIME * 1000;
    uint8_t message_error = 0;
    while(millis() < time_to_end_recieving){
        if((message_error = receiver.receiveMessage(messageBytesBuffer)) == RECIEVE_ERROR_SUCCESS) // checks if error code is a success
            break;
    }

    if (!message_error)
    {
        // <- decode message, and see what to do with it
        uint8_t result = decodeAndAnalyseMessage(messageBytesBuffer, messageBuffer);

        // <- decide what to do
        if (result == MESSAGE_SEEN_CODE) // message has already been seen before
            ; //do nothing? (include read reciepts, to see if message has gone further)
        if (result == MESSAGE_TO_FORWARD_CODE)
        {
            sender.startSend();
            sender.sendPackets(messageBytesBuffer);
        }
        if (result == MESSAGE_TO_ME_CODE)
            Serial.println(messageBuffer); //display message on serial
    }

    // <- send message from serial?

    

    SerialInput(myQueue); // get user input
    sender.startSend();
    while(!myQueue.isEmpty()) // repeat for every message in queue
    {
        struct packet packet_to_send;
        resetBuffers();
        SerialOutput(myQueue, messageBuffer);
        packet_to_send = encode_message_to_send(my_address, 0, messageBuffer);

        sender.sendPackets(packet_to_send.encoded_bytes);
    }
}