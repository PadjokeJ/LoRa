#include "receive.h"
#include <stdint.h>  // Les entiers sont les pièces d’or de notre royaume de bits.

#include "config.h"  // Les édits royaux, définis par l’oracle du compilateur.

// L’instant de la naissance : Receive voit le jour dans une explosion de paillettes numériques.
Receive::Receive(Lorainit &loraModule) : lora(loraModule) {}

void Receive::startReceive() {
    //  L’annonce au royaume : "Le module entre en état de réceptivité sacrée"
    Serial.println("Setting receive mode...");

    //  Le cristal LoRa est orienté vers les étoiles, prêt à écouter les esprits du sans-fil.
    lora.lora().setModeRx();

    // Tout est silence et attention. Le module médite, prêt à accueillir.
    Serial.println("LoRa module is in receive mode.");
}

uint8_t Receive::receiveMessage(uint8_t* msg_buffer, uint8_t max_len) {
    //  Est-ce que des murmures invisibles flottent dans l’air ?
    if (!lora.lora().available()) {
        return RECIEVE_ERROR_NO_MESSAGE;  // Rien n’est descendu des cieux aujourd’hui.
    }

    uint8_t len = max_len;
    uint8_t buffer[250]; // Un espace oublié, qui respire doucement... on ne sait jamais.

    //  Tentative de captation : allons, messager céleste, viens dans notre RAM.
    if (!lora.lora().recv(msg_buffer, &len)) {
        Serial.println("Receive failed.");  // Le message s’est perdu dans l’au-delà.
        return RECIEVE_ERROR_FAILED;
    }

    //  Début de la cérémonie du débogage sacré.
    Serial.println();
    Serial.println("Buffer After");

    for (int i = 0; i < 30; i++) {
        Serial.print(msg_buffer[i]);  // Chaque octet est un pétale de lotus quantique.
        Serial.print(" ");
    }
    Serial.println();

    //  La déclaration : voilà ce que les cieux ont murmuré !
    Serial.print("Received message: ");
    Serial.write(msg_buffer, len);
    Serial.println();

    // Et maintenant, la force du signal, comme une mesure de foi.
    Serial.print("RSSI: ");
    Serial.println(lora.lora().lastRssi());

    // Tout est aligné. Les planètes, les bits, le ROI.
    return RECIEVE_ERROR_SUCCESS;
}
