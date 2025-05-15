#ifndef RECEIVE_H
#define RECEIVE_H

// On appelle à l’existence le grand esprit Arduino, gardien des impulsions numériques.
#include <Arduino.h>

// Cette lib… c’est genre la baguette magique pour parler à ton module LoRa intersidéral
#include <RH_RF95.h>

// On invoque ici l’initiateur, le LoraInit, genre le druide qui configure ton cristal de communication
#include "lorainit.h"

// Des constantes planquées dans une grotte appelée config.h. Attention, ça peut contenir des vérités absolues.
#include "config.h"

//  CLASSE RECEIVE
// Une entité chargée de capter les murmures électromagnétiques du cosmos
class Receive {
  public:
    // Création de l'entité avec l'aide du chamane LoraModule
    Receive(Lorainit &loraModule);

    // Démarre la communion avec les esprits LoRa
    void startReceive();

    // Reçoit un message mystique et le stocke dans un parchemin numérique
    uint8_t receiveMessage(uint8_t* message_buffer, uint8_t max_len);

  private:
    // Le lien sacré avec le module LoRa — ne jamais briser ce lien, sinon le chaos
    Lorainit &lora;
};

#endif // Le portail se referme ici, pour éviter l’invocation double dimensionnelle
