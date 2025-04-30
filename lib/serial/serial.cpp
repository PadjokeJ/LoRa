#include <cppQueue.h> // Inclure la bibliothèque cppQueue pour gérer les files d'attente
#include <stdint.h>
#include <SPI.h>
#include "../../src/config.h"

#include "serial.h"

uint8_t receivedData[MAX_MESSAGE_LEN]; // Définir un tableau pour stocker les données reçues
int i = 0;


void SerialInput(cppQueue queue) {
  char c = '\0';

  while(Serial.available() > 0) {
    c = Serial.read();
    if(c == '\n' || c == '\r') break; // un retour à la ligne ajoute le message à la queue

    receivedData[i++] = c;
  }

  
  if(i > 0 || i < MAX_MESSAGE_LEN - 1) {
    if (!queue.isFull()) { // Vérifier que la file d'attente n'est pas pleine
      queue.push(receivedData); // Ajouter les données à la file d'attente
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

void SerialOutput(cppQueue queue, char* array_buffer) {
  uint8_t* dataToSend;
  if (!queue.isEmpty()) { // Vérifier que la file d'attente n'est pas vide
    dataToSend; // Variable pour stocker l'élément à envoyer
    queue.pop(&dataToSend); // Retirer l'élément en tête de la file d'attente
    Serial.println((char)dataToSend); // Envoyer l'élément via la communication série

    array_buffer = (char*)dataToSend;
  }
}

#ifndef __MAIN_SCRIPT__
cppQueue myQueue(10, sizeof(uint8_t), FIFO);

char message_buffer[MAX_MESSAGE_LEN - 6];

void setup() {
  Serial.begin(9600); // Configurer la communication série avec un débit en bauds de 9600
}

void loop() {
  SerialInput(myQueue); // Appeler la fonction de gestion des entrées série
  SerialOutput(myQueue, message_buffer); // Appeler la fonction de gestion des sorties série
}
#endif