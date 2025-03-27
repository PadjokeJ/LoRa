#include <SPI.h>
#include <RH_RF95.h>

// Define the LoRa module pins
#define RFM95_CS 10  // NSS
#define RFM95_RST 9  // Reset
#define RFM95_INT 2  // DIO0

// Frequency (e.g., 868 MHz for Europe, 915 MHz for US)
#define RF95_FREQ 868.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
    Serial.begin(9600);
    while (!Serial);

    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    if (!rf95.init()) {
        Serial.println("LoRa module initialization failed!");
        while (1);
    }

    Serial.println("LoRa module initialized.");

    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("Failed to set frequency.");
        while (1);
    }

    Serial.print("Frequency set to: ");
    Serial.println(RF95_FREQ);

    rf95.setTxPower(13, false);  // Transmit power (default 13 dBm)
}

void loop() {
    Serial.println("Sending: MessageToutCool-OC-INFO");
    uint8_t message[] = "MessageToutCool-OC-INFO";

    rf95.send(message, sizeof(message));
    rf95.waitPacketSent();

    Serial.println("Message Sent!");

    delay(1000);  // Send every second
}