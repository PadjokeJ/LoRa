#include <SPI.h>
#include <RH_RF95.h>

#include "packet.h"
#include "decode.h"
#include "encode.h"

#define RFM95_CS      10  // Chip Select pin
#define RFM95_RST     9   // Reset pin
#define RFM95_INT     2   // Interrupt pin (DIO0)
#define RF95_FREQ     868.0 // Frequency (set according to your region)

#define SENDER

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
  Serial.println();
  Serial.println("LoRa initialized successfully!");

  rf95.setFrequency(RF95_FREQ);  // Set frequency
  rf95.setTxPower(23, false);  // Set transmit power
}

void loop() {
  #ifdef SENDER
  uint8_t msgbuf[245] = {0};
  
  char c = 0;
  int index = 0;
  while(1){
    c = Serial.read();
    if (c != -1){
      Serial.print((int)c);
      Serial.print(", ");

      if (c >= 32)
        msgbuf[index++] = c;

      if (c == 10)
      {
        msgbuf[index++] = 0;
        break;
      }
    }
  }
  uint8_t outbuf[251] = {0};
  
  if (msgbuf[0] >= 32){
    Serial.println();
    Serial.println((char*)msgbuf);
    struct packet send = encode_message_to_send((uint8_t) 65, (uint8_t) 69, msgbuf, outbuf);

    for(int i = 0; i < 30; i++){
      Serial.print(outbuf[i]);
      Serial.print(", ");
    }

    rf95.send(outbuf, strlen(outbuf));  // Send message
    rf95.waitPacketSent();  // Wait until the message is sent
    Serial.println("Message sent!");
    delay(1000);  // Wait 1 second before sending the next message
  }
  #endif

  #ifndef SENDER
  uint8_t inbuf[251] = {0};
  uint8_t lenMSG = sizeof(inbuf);
  if (rf95.available()) {
    if(rf95.recv(inbuf, &lenMSG))
      {
        Serial.println();
        Serial.println("Recieved a new message!");

        uint8_t message_buffer[251] = {0};
        struct packet rec;
        rec = decode(inbuf, message_buffer);
        Serial.print("┌From: ");
        Serial.println(rec.source);
        Serial.print("├To: ");
        Serial.println(rec.destination);
        Serial.print("└Message: ");
        Serial.println((char*)message_buffer);
      }
  }
  #endif
}