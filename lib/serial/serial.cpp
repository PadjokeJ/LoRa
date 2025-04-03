#include <SPI.h>
#include <LoRa.h>

const long frequency = 915E6; // Fréquence LoRa (doit correspondre à l'émetteur)

void serialOutput(String message) {
  Serial.print("Received Data: ");
  Serial.println(message);
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver Initializing...");
  
  if (!LoRa.begin(frequency)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.println("LoRa Receiver Ready");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String receivedMessage = "";
    while (LoRa.available()) {
      receivedMessage += (char)LoRa.read();
    }
    serialOutput(receivedMessage);
  }
