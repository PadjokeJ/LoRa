#include <SPI.h>
#include <RH_RF95.h>

#include "packet.h"
#include "decode.h"

#include "encode.h"

#define RFM95_CS      10  // Chip Select pin
#define RFM95_RST     9   // Reset pin
#define RFM95_INT     2   // Interrupt pin (DIO0)
#define RF95_FREQ     868.0 // Frequency (set according to your region)

//#define SENDER

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // Manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  Serial.begin(9600);
  while (!Serial);

  if (!rf95.init()) {
    Serial.println("LoRa initialization failed!");
    while (1);
  }
  Serial.println("LoRa initialized successfully!");

  rf95.setFrequency(RF95_FREQ);  // Set frequency
  rf95.setTxPower(23, false);  // Set transmit power

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  #ifdef SENDER
  uint8_t outbuf[251] = {0};

  struct packet send = encode_message_to_send((uint8_t) 65, (uint8_t) 0, "Hello world!", outbuf);

  Serial.println();

  digitalWrite(LED_BUILTIN, HIGH);
  sender.sendPackets(outbuf);  // Send message
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Message sent!");
  delay(1000);  // Wait 1 second before sending the next message
  #endif


  #ifndef SENDER
  uint8_t inbuf[251] = {0};
  uint8_t lenMSG = sizeof(inbuf);
  if (rf95.available()) {
    if(rf95.recv(inbuf, &lenMSG))
      {
        for(int i = 0; i < 30; i++){
          Serial.print(inbuf[i]);
          Serial.print(", ");
        }
        Serial.println();
      }
  }
  #endif
}