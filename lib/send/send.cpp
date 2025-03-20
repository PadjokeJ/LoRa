 void sendLoRaMessage(String message) {
  // Début de la transmission
  LoRa.beginPacket();
  
  // Envoi du message
  LoRa.print(message);
  
  // Fin de la transmission
  LoRa.endPacket();     

  // Affichage du message envoyé sur le moniteur série
  Serial.print("Message envoyé : ");
  Serial.println(message);