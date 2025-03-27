#include"lorainit.h"
#include <SPI.h>
#include <RH_RF95.h>

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