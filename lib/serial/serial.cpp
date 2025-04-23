#include <cppQueue.h> // Inclure la bibliothèque cppQueue pour gérer les files d'attente
#include <stdint.h>
#include <SPI.h>

#include "serial.h"

// Initialisation de la file d'attente avec une capacité maximale de 10 éléments, type uint8_t, mode FIFO
cppQueue myQueue(10, sizeof(uint8_t), FIFO);
uint8_t receivedData[255]; // Définir un tableau pour stocker les données reçues
int i = 0;


void SerialInput() {
  char c = '\0';

  while(Serial.available() > 0) {
    c = Serial.read();
    receivedData[i++] = c;
  }

  if(c == '\n' || c == '\r') {
    if(i > 0) {
      if (!myQueue.isFull()) { // Vérifier que la file d'attente n'est pas pleine
        myQueue.push(receivedData); // Ajouter les données à la file d'attente
        Serial.print(">>>> Queued this: "); // Afficher un message pour les données ajoutées
        
        for(int j = 0; j < i; j++)
          Serial.print(char(receivedData[j])); // Convertir et afficher les caractères reçus
        
        Serial.println(""); // Passer à la ligne suivante
      } 
      else {
        Serial.println("Erreur : La file est pleine !"); // Afficher une erreur si la file est pleine
      }
    } 
    receivedData[i++] = '\0';
    i = 0;
  }
}

void SerialOutput() {
  if (!myQueue.isEmpty()) { // Vérifier que la file d'attente n'est pas vide
    uint8_t dataToSend; // Variable pour stocker l'élément à envoyer
    myQueue.pop(&dataToSend); // Récupérer et retirer l'élément en tête de la file d'attente
    Serial.println(dataToSend); // Envoyer l'élément via la communication série
  }
}

#ifndef __MAIN_SCRIPT__
void setup() {
  Serial.begin(9600); // Configurer la communication série avec un débit en bauds de 9600
}

void loop() {
  SerialInput(); // Appeler la fonction de gestion des entrées série
  SerialOutput(); // Appeler la fonction de gestion des sorties série
}
#endif