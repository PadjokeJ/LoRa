#include "receive.h"

#include <stdint.h>

#include "../../config.h"

Receive::Receive(Lorainit &loraModule) : lora(loraModule) {}

void Receive::startReceive() {
    Serial.println("Setting recieve mode...");
    lora.lora().setModeRx();  // Mettre le module en mode réception
    Serial.println("LoRa module is in receive mode.");
}

uint8_t Receive::receiveMessage(uint8_t* message_buffer) {
    if (!lora.lora().available()) {  // Check if a packet is received
        return RECIEVE_ERROR_NO_MESSAGE;
    }
    uint8_t len = sizeof(message_buffer);

    if (!lora.lora().recv(message_buffer, &len)) {  // Receive packet
        Serial.println("Receive failed.");
        return RECIEVE_ERROR_FAILED;
    }

    Serial.print("Received message: ");
    Serial.write(message_buffer, len);  // Print message
    Serial.println();

    Serial.print("RSSI: ");
    Serial.println(lora.lora().lastRssi());  // Print signal strength

    return RECIEVE_ERROR_SUCCESS;
}