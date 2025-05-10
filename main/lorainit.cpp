#include "lorainit.h"
#include <RH_RF95.h>

#include "config.h"


//-------------------------Constructeur de Lorainit------------------------
Lorainit::Lorainit(int ssPin, int freq){
    _ssPin = ssPin;
    _freq = freq;
    _rf95 = new RH_RF95(_ssPin);
}

void Lorainit::init() {
    Serial.begin(9600); //initialise la communication avec l'ordi au cas ou
    while (!Serial);    // attent la connection serial avec l'ordi

    // Initialisation du LoRa
    if (!_rf95->init()) {
        Serial.println("LoRa module initialization failed!");
        while (1);      // Boucle infinie en cas d’échec
    }

    // Configuration de la fréquence
    if (!_rf95->setFrequency(_freq)) {
        Serial.println("Failed to set frequency.");
        while (1);  // Boucle infinie en cas d’échec
    }

    Serial.print("LoRa operating on frequency: ");
    Serial.println(_freq);

    // Configuration de la puissance d'émission
    _rf95->setTxPower(LORA_STRENGTH, false);

    Serial.println("LoRa module initialized.");
}

RH_RF95& Lorainit::lora() {
    return *_rf95;
}