#include <Arduino.h>
#include "LoRaReceiver/LoRaReceiver.h"

LoRaReceiver loraReceiver;

void setup() {
    Serial.begin(9600);
    loraReceiver.begin();
}

void loop() {
    loraReceiver.loop();
    
    char* message = loraReceiver.getMessage();
    if (message != nullptr) {
        Serial.println("Message reçu : ");
        Serial.println(message);
    }
}
