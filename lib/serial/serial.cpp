#include <SPI.h>
#include <LoRa.h>

const long frequency = 915E6; // Match the transmitter's frequency

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial Monitor to open

  Serial.println("LoRa Receiver with Optimized Settings");

  if (!LoRa.begin(frequency)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  // Apply the same configuration as the transmitter:
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  
  Serial.println("LoRa Receiver Ready");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }
    Serial.print("Received packet: ");
    Serial.println(received);
  }
}
