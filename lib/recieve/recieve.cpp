#include "recieve.h"

#include <stdint.h>

#include "../src/config.h"

Receive::Receive(Lorainit &loraModule) : lora(loraModule) {}

void Receive::startReceive() {
    Serial.println("Setting recieve mode...");
    lora.lora().setModeRx();  // Mettre le module en mode réception
    Serial.println("LoRa module is in receive mode.");
}

void Receive::receiveMessage() {
    if (lora.lora().available()) {  // Check if a packet is received
        unsigned char recieved_message[MAX_BYTES_LEN];  // Buffer to store received data
        unsigned char len = sizeof(recieved_message);

        if (lora.lora().recv(recieved_message, &len)) {  // Receive packet
            Serial.print("Received message: ");
            Serial.write(recieved_message, len);  // Print message
            Serial.println();

            Serial.print("RSSI: ");
            Serial.println(lora.lora().lastRssi());  // Print signal strength
        } else {
            Serial.println("Receive failed.");
        }
    }
}