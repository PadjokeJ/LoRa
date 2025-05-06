#include "receive.h"

#include <stdint.h>

#include "config.h"

Receive::Receive(Lorainit &loraModule) : lora(loraModule) {}

void Receive::startReceive() {
    Serial.println("Setting recieve mode...");
    lora.lora().setModeRx();  // Mettre le module en mode réception
    Serial.println("LoRa module is in receive mode.");
}

uint8_t Receive::receiveMessage(uint8_t* msg_buffer) {
    //Serial.println("BUFFER BEFORE");
    //Serial.println();
    //for(int i = 0; i < 30; i++)
    //{
    //    Serial.print(msg_buffer[i]);
    //    Serial.print(" ");
    //}
    //Serial.println();
    //Serial.println("Buffer After");
    if (!lora.lora().available()) {  // Check if a packet is received
        return RECIEVE_ERROR_NO_MESSAGE;
    }
    uint8_t len = max_len;
    uint8_t buffer[250];

    if (!lora.lora().recv(msg_buffer, &len)) {  // Receive packet
        Serial.println("Receive failed.");
        return RECIEVE_ERROR_FAILED;
    }

    Serial.println();
    Serial.println("Buffer After");

    for(int i = 0; i < 30; i++)
    {
        Serial.print(msg_buffer[i]);
        Serial.print(" ");
    }
    Serial.println();

    Serial.print("Received message: ");
    Serial.write(msg_buffer, len);  // Print message
    Serial.println();

    Serial.print("RSSI: ");
    Serial.println(lora.lora().lastRssi());  // Print signal strength

    return RECIEVE_ERROR_SUCCESS;
}