#include <SPI.h>
#include <RH_RF95.h>

#include "packet.h"
#include "decode.h"
#include "lorainit.h"
#include "send.h"
#include "receive.h"
#include "encode.h"
#include "memory.h"

#define RFM95_CS      10  // Chip Select pin
#define RFM95_RST     9   // Reset pin
#define RFM95_INT     2   // Interrupt pin (DIO0)
#define RF95_FREQ     868.0 // Frequency (set according to your region)

//#define SENDER

Lorainit lora = Lorainit(RFM95_CS, RF95_FREQ, RFM95_INT);
Send sender = Send(lora);
Receive receiver = Receive(lora);

uint16_t* seen_message_IDs[10];

void setup() {
  lora.init();
  //while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  uint8_t outbuf[251] = {0};
  uint8_t message_buf[245]  = {0};
  uint8_t mLen;

  #ifdef SENDER
  struct packet send = encode_message_to_send((uint8_t) 65, (uint8_t) 0, "Hello world!", outbuf);

  Serial.println();

  digitalWrite(LED_BUILTIN, HIGH);
  sender.sendPackets(outbuf);  // Send message
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Message sent!");
  delay(1000);  // Wait 1 second before sending the next message
  #endif
  #ifndef SENDER
  if(receiver.receiveMessage(outbuf, mLen) == RECIEVE_ERROR_SUCCESS){
    struct packet decoded_message = decode(outbuf, message_buf);
    Serial.print("Message: ");
    Serial.println((char *)message_buf);
  }
  #endif
}