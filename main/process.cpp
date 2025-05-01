#include <LoRa.h>
#include <Queue.h>

#define SS 10
#define RST 9
#define DIO0 2

const int OUR_ID = 0x01;

// File d'attente pour stocker les paquets reçus
Queue<String> receiveQueue(10);

#ifndef __MAIN_SCRIPT__
void setup() {
    Serial.begin(9600);
    while (!Serial);
   
    // Initialisation du module LoRa
    LoRa.setPins(SS, RST, DIO0);
    if (!LoRa.begin(915E6)) {  // Fréquence 915MHz (à ajuster selon région)
        Serial.println("Échec de l'initialisation LoRa");
        while (1);
    }
    Serial.println("LoRa initialisé avec succès");
}


void loop() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        // Lire le paquet
        String receivedData = "";
        while (LoRa.available()) {
            receivedData += (char)LoRa.read();
        }
       
        // Vérifier si c'est notre paquet
        int srcID = receivedData.substring(0, 2).toInt();
        if (srcID == OUR_ID) {
            Serial.println("Paquet reçu pour nous : " + receivedData);
            receiveQueue.push(receivedData);
            processReceive();
        } else {
            Serial.println("Paquet ignoré (pas pour nous)");
        }
    }
}
#endif

void processReceive() {
    if (!receiveQueue.isEmpty()) {
        String packet = receiveQueue.pop();
        Serial.println("Traitement du paquet : " + packet);
        decode(packet);
    }
}

void decode(String packet) {
    Serial.println("Décodage du paquet : " + packet);
}
