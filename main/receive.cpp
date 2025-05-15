#include "receive.h"

// Inclusion de la bibliothèque standard des types d'entiers, pour une gestion rigoureuse de la mémoire.
#include <stdint.h>

// Inclusion des constantes, vraisemblablement des codes d’erreur et paramètres définis en amont.
#include "config.h"

// Le constructeur de la classe Receive : il reçoit en legs une référence à un module LoRa déjà prêt à servir.
Receive::Receive(Lorainit &loraModule) : lora(loraModule) {}

void Receive::startReceive() {
    // Annonce solennelle de la mise en place du mode réception.
    Serial.println("Setting receive mode...");

    // Ordre donné au module LoRa de passer à l’état d’écoute, noble posture pour tout récepteur.
    lora.lora().setModeRx();

    // Confirmation à l’utilisateur : le dispositif est désormais en état de recevoir.
    Serial.println("LoRa module is in receive mode.");
}

uint8_t Receive::receiveMessage(uint8_t* msg_buffer, uint8_t max_len) {
    // Vérification préliminaire : y a-t-il un message à recevoir en ces lieux ?
    if (!lora.lora().available()) {
        // Point de message, point de réception.
        return RECIEVE_ERROR_NO_MESSAGE;
    }

    uint8_t len = max_len;
    uint8_t buffer[250]; // Tampon inutile dans le cas présent, mais laissé là par prudence ou oubli.

    // Tentative de réception : le module se voit confier la noble tâche de remplir le tampon fourni.
    if (!lora.lora().recv(msg_buffer, &len)) {
        // Hélas ! la réception a échoué.
        Serial.println("Receive failed.");
        return RECIEVE_ERROR_FAILED;
    }

    // Séquence de débogage : inspection des octets reçus.
    Serial.println();
    Serial.println("Buffer After");

    for (int i = 0; i < 30; i++) {
        Serial.print(msg_buffer[i]);
        Serial.print(" ");
    }
    Serial.println();

    // Annonce du message reçu, pour la plus grande joie du programmeur.
    Serial.print("Received message: ");
    Serial.write(msg_buffer, len);
    Serial.println();

    // Indication de la puissance du signal, reflet de la qualité de la transmission.
    Serial.print("RSSI: ");
    Serial.println(lora.lora().lastRssi());

    // Tout s’est déroulé avec succès.
    return RECIEVE_ERROR_SUCCESS;
}
