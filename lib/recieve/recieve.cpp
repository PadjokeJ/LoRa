#include "recieve.h"
#include <SPI.h>
#include <RH_RF95.h>

// Define LoRa module pins


Receive::Receive(int ssPin, int rstPin, int freq) {
  _ssPin = ssPin;  // NSS
  _rstPin = rstPin;  // reset
  _freq = freq;  // freq
  rf95 = RH_RF95(ssPin, rstPin);  // Initialize le LoRa module
}


void Receive::init() {
    Serial.begin(9600);
    while (!Serial);

    // Reset the LoRa module
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    // Initialize the LoRa module
    if (!rf95.init()) {
        Serial.println("LoRa module initialization failed!");
        while (1);
    }

    Serial.println("LoRa module initialized.");

    // Set the frequency
    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("Failed to set frequency.");
        while (1);
    }

    Serial.print("Listening on frequency: ");
    Serial.println(RF95_FREQ);

    // Set receiver mode
    rf95.setTxPower(13, false);
}

void Receive::receiveMessage() {
    if (rf95.available()) {  // Check if a packet is received
        unsigned char recieved_message[RH_RF95_MAX_MESSAGE_LEN];  // Buffer to store received data
        unsigned char len = sizeof(recieved_message);

        if (rf95.recv(recieved_message, &len)) {  // Receive packet
            Serial.print("Received message: ");
            Serial.write(recieved_message, len);  // Print message
            Serial.println();

            Serial.print("RSSI: ");
            Serial.println(rf95.lastRssi());  // Print signal strength
        } else {
            Serial.println("Receive failed.");
        }
    }
}
