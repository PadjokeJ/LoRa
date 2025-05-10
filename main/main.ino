#include <SPI.h>
#include <RH_RF95.h>

#include "packet.h"
#include "decode.h"
#include "encode.h"
#include "config.h"

#define RFM95_CS      10  // Chip Select pin
#define RFM95_RST     9   // Reset pin
#define RFM95_INT     2   // Interrupt pin (DIO0)
#define RF95_FREQ     868.0 // Frequency (set according to your region)


RH_RF95 rf95(RFM95_CS, RFM95_INT);
int step = STATE_ASK_ADDRESS;
int dest = 0;

int to_int(uint8_t* ch){
  int i = 0;
  while(ch[i++]);

  Serial.print("TO_INT: 1 ; ");
  Serial.println(i);

  int num = 0;
  i-= 2;
  int max = i;
  Serial.print("TO_INT: ch ; ");
  Serial.println(ch[i]);
  while(ch[i]){
    if(ch[i] >= 48 && ch[i] <= 57){
      int tmp = 1;
      for(int j = 0; j < max - i; j++)
        tmp *= 10;
      Serial.print("TO_INT: tmp ; ");
      Serial.println(tmp);
      num += tmp * (ch[i] - 48);
    }else{
      return -1;
    }
    i--;
  }
  if (num > 255) return -1;
  return num;
}

void try_recieve(){
  uint8_t inbuf[251] = {0};
  uint8_t lenMSG = sizeof(inbuf);
  if (rf95.available()) {
    if(rf95.recv(inbuf, &lenMSG)){
        Serial.println();
        Serial.println("Recieved a new message!");

        uint8_t message_buffer[251] = {0};
        struct packet rec;
        rec = decode(inbuf, message_buffer);
        if(rec.destination == MY_ADDRESS){
          Serial.print("┌From: ");
          Serial.println(rec.source);
          Serial.print("├To: ");
          Serial.println(rec.destination);
          Serial.print("└Message: ");
          Serial.println((char*)message_buffer);
        }
      }
  }
}

void get_user_input(int index, uint8_t* msgbuf){
  char c = 0;
  while(1){
    c = Serial.read();
    try_recieve();
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
}

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
  Serial.println();

  Serial.print("LoRa is situated on network at address ");
  Serial.println(MY_ADDRESS);
  Serial.println();


  rf95.setFrequency(RF95_FREQ);  // Set frequency
  rf95.setTxPower(23, false);  // Set transmit power
}

void loop() {
  uint8_t msgbuf[245] = {0};

  if(step == STATE_ASK_ADDRESS)
  {
    Serial.println("Please input message destination address");
    step++;
  }
  if(step == STATE_ASK_MESSAGE)
  {
    Serial.println("Please input message");
    step++;
  }

  int index = 0;

  get_user_input(index, msgbuf);

  
  if (step == STATE_INPUT_ADDRE){
    dest = to_int(msgbuf);
    if (dest == -1)
    {
      step = 0;
      Serial.println(" /!\\ Invalid address /!\\ ");
    }
    if(dest >= 0)
    {
      Serial.print("Set address -> ");
      Serial.println(dest);
      step++;
    }
  }

  uint8_t outbuf[251] = {0};
  
  if(step == STATE_INPUT_MESSA){
    if (msgbuf[0] >= 32){
      Serial.println();
      Serial.println((char*)msgbuf);
      struct packet send = encode_message_to_send(MY_ADDRESS, (uint8_t) dest, msgbuf, outbuf);

      for(int i = 0; i < 30; i++){
        Serial.print(outbuf[i]);
        Serial.print(", ");
      }

      rf95.send(outbuf, strlen(outbuf));  // Send message
      rf95.waitPacketSent();  // Wait until the message is sent
      Serial.println("Message sent!");
      delay(1000);  // Wait 1 second before sending the next message

      step = 0;
    }
  }
}