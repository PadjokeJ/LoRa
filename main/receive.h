#ifndef RECEIVE_H
#define RECEIVE_H

// Ouais, on a besoin d'Arduino pour que ça compile
#include <Arduino.h>

// La lib pour causer avec le module LoRa
#include <RH_RF95.h>

// On inclut la classe de config/init du module LoRa
#include "lorainit.h"

// Fichier de config, probablement des #define qui trainent
#include "config.h"

// Classe qui s’occupe de la réception LoRa
class Receive {
  public:
    // Constructeur, on lui file un objet Lorainit (qu’on a déjà préparé ailleurs)
    Receive(Lorainit &loraModule);

    // Démarre la réception (en vrai ça doit juste faire un setup du module)
    void startReceive();

    // Fonction qui récupère un message si y en a un
    // Elle le fout dans le buffer qu’on lui passe
    // Renvoie la taille du message reçu (ou 0 si y a rien probablement)
    uint8_t receiveMessage(uint8_t* message_buffer, uint8_t max_len);

  private:
    // Référence vers l’objet qui gère le module LoRa
    Lorainit &lora;
};

#endif // ça évite que ce soit inclus deux fois
