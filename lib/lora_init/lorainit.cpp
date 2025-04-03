#include "lorainit.h"

Lorainit::Lorainit(int ssPin, int freq) 
  : rf95(ssPin) {  // Initialisation de RH_RF95 avec NSS uniquement
    _ssPin = ssPin;
    _freq = freq;
}

void Lorainit::init() {
    Serial.begin(9600);
    while (!Serial);

    // Initialisation du LoRa
    if (!rf95.init()) {
        Serial.println("LoRa module initialization failed!");
        while (1);
    }

    // Configuration de la fréquence
    if (!rf95.setFrequency(_freq)) {
        Serial.println("Failed to set frequency.");
        while (1);
    }

    Serial.print("LoRa operating on frequency: ");
    Serial.println(_freq);

    // Configuration de la puissance d'émission
    rf95.setTxPower(21, false);

    Serial.println("LoRa module initialized.");
}

RH_RF95& Lorainit::lora() {
    return rf95; // Return a reference to the rf95 object
}