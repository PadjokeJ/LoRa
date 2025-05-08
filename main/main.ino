#include <SPI.h>
#include <RH_RF95.h>

#include "packet.h"
#include "decode.h"
#include "lorainit.h"
#include "send.h"
#include "encode.h"

#define RFM95_CS      10  // Chip Select pin
#define RFM95_RST     9   // Reset pin
#define RFM95_INT     2   // Interrupt pin (DIO0)
#define RF95_FREQ     868.0 // Frequency (set according to your region)

Lorainit lora = Lorainit(RFM95_CS, RF95_FREQ, RFM95_INT);
Send sender = Send(lora);

void setup() {
  lora.init();
  while (!Serial);
}

void loop() {
  uint8_t outbuf[251];
  struct packet send = encode_message_to_send((uint8_t) 65, (uint8_t) 0, "Hello world!", outbuf);

  for(int i = 0; i < 30; i++){
    Serial.print(outbuf[i]);
    Serial.print(", ");
  }

  sender.sendPackets(outbuf);  // Send message
  Serial.println("Message sent!");
  delay(1000);  // Wait 1 second before sending the next message
}