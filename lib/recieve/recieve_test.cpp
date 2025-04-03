#include <SPI.h>
#include <RH_RF95.h>
#include <cppQueue.h>

// Define LoRa module pins
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

// LoRa Frequency
#define RF95_FREQ 868.0  

// Create LoRa object
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// QUEUE MANAGEMENT
#define MAX_MESSAGE_SIZE 100  // Reduced message size
#define QUEUE_SIZE 5        // Reduced queue size

// Queue to store messages as raw byte arrays
cppQueue messageQueue(QUEUE_SIZE, MAX_MESSAGE_SIZE, LIFO);  // Queue of integers
cppQueue lengthQueue(sizeof(unsigned char), QUEUE_SIZE, LIFO);  


void setup() {
    Serial.begin(9600);
    while (!Serial);

    // Reset the LoRa module
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

    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("Failed to set frequency.");
        while (1);
    }

    Serial.println("LoRa module initialized.");

}

void loop() {
  //sendtest();
    recieve();
    // Process messages from the queue
    if (!messageQueue.isEmpty() && !lengthQueue.isEmpty()) {
        unsigned char messageLength;
        byte dequeuedMessage[MAX_MESSAGE_SIZE];

        lengthQueue.pop(&messageLength);  // Pop message length
        messageQueue.pop(dequeuedMessage);  // Pop actual message

        Serial.print("Dequeued Message: ");
        Serial.write(dequeuedMessage, messageLength);
        Serial.println();
    }

    delay(100);  // Small delay to control loop speed
}
