#include <SPI.h>
#include <LoRa.h>  // Bibliothèque LoRa

#define MAX_QUEUE_SIZE 10  // Taille max de la file d'attente

char* sendQueue[MAX_QUEUE_SIZE]; // Tableau pour stocker les messages
int rear = 0; // Position du dernier élément dans la file d'attente








void AddSentQueue(char* message) {
    sendQueue[rear] = message; // Ajoute le message à l'arrière de la file
    rear++; // Incrémente l'indice rear pour la prochaine insertion
}






void Send() {
    if (rear == 0) { // Vérifie si la file est vide
        Serial.println("Aucun message à envoyer.");
        return;
    }

    Serial.print("message envoyé : "); // Ajoute paquet ici !!!!
    Serial.println(sendQueue[0]); // Affiche le message envoyé dans le moniteur série

    // Envoi du message avec LoRa (doc)
    LoRa.beginPacket();   // Démarre un paquet LoRa
    LoRa.print(sendQueue[0]); // Ajoute le message au paquet
    LoRa.endPacket();     // Envoie le paquet

    // replace les autres messages
    for (int i = 0; i < rear - 1; i++) {
        sendQueue[i] = sendQueue[i + 1];
    }

    rear--; // Réduit la taille de la file d'attente après l'envoi
}


void loop() {
    AddSentQueue("Message"); 

    Send(); 
    
}