#include "send.h"

#include <SPI.h>
#include <RH_RF95.h>

#include "../src/config.h"

RH_RF95 rf95(RFM95_CS, RFM95_INT);

#ifndef __MAIN_SCRIPT__
void setup() {
    Serial.begin(9600);
    while (!Serial);

    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    if (!rf95.init()) {
        Serial.println("LoRa module initialization failed!");
        while (1);
    }

    Serial.println("LoRa module initialized.");

    if (!rf95.setFrequency(LORA_FREQ)) {
        Serial.println("Failed to set frequency.");
        while (1);
    }

    Serial.print("Frequency set to: ");
    Serial.println(LORA_FREQ);

    rf95.setTxPower(LORA_STRENGTH, false);  // Transmit power (from config file -> 20 dBm)
}

void loop() {
    Serial.println("Sending: MessageToutCool-OC-INFO");
    uint8_t message[] = "MessageToutCool-OC-INFO";

    rf95.send(message, sizeof(message));
    rf95.waitPacketSent();

    Serial.println("Message Sent!");

    delay(1000);  // Send every second
}
#endif