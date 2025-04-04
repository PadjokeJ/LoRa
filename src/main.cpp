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


#define LORA_SS 10     
#define LORA_FREQ 868  

Lorainit lora(LORA_SS, LORA_FREQ);
Receive receiver;
Send sender;

void setup() {
    lora.init();
}

void loop() {
    
}
