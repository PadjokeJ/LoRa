/**
 * Main code to send and receive messages with Dragino-dev lorawan devices
 * Works in a network
 *
 * Code mostly redacted by padjokej.dev
**/

#include <SPI.h> // library for communicating with lora shield
#include <RH_RF95.h> // library for base lorawan dragino shield functions

#include "packet.h" // include packet library
#include "decode.h" // include decode library for decoding received packets
#include "encode.h" // include encode library for encoding packets to send
#include "config.h" // include config data
#include "memory.h" // include memory library to detect wether we have seen a message, to avoid sending loops

#define RFM95_CS      10  // Chip Select pin
#define RFM95_RST     9   // Reset pin
#define RFM95_INT     2   // Interrupt pin (DIO0)
#define RF95_FREQ     868.0 // Frequency (set according to your region)


RH_RF95 rf95(RFM95_CS, RFM95_INT); // declare our LoRa-wan object
int step = STATE_ASK_ADDRESS; // state machine step, to change type of inputs
int dest = 0; // temporary address for destination

#define SEEN_MESSAGES_ARRAY_SIZE 20 // size for how many message ids we store in memory for memory.h
uint16_t seen_messages[SEEN_MESSAGES_ARRAY_SIZE] = {0}; // declare the array in which we will store

// function to transform a string into an integer, if an error occurs, returns -1
int to_int(uint8_t* ch){
  int i = 0;
  while(ch[i++]); // go through every character in string, until end of string, to start at end of string

  // DEBUG
  //Serial.print("TO_INT: 1 ; ");
  //Serial.println(i);

  int num = 0; // where we will store number
  i-= 2; // go back to last number in string
  int max = i; // set this as how many chars are in string
  //Serial.print("TO_INT: ch ; ");
  //Serial.println(ch[i]);
  while(ch[i]){ // loop over every character backwards
    if(ch[i] >= 48 && ch[i] <= 57){
      int tmp = 1;
      for(int j = 0; j < max - i; j++)
        tmp *= 10 * j;
      num += tmp * (ch[i] - 48);
    }else{
      return -1; // if char is not equivalent to number
    }
    i--;
  }
  if (num > 255) return -1; //if our number is bigger than a potential address -> error
  return num;
}

// code in which we try to receive a message
void try_recieve(){
  uint8_t inbuf[251] = {0}; // buffer to store message
  uint8_t lenMSG = sizeof(inbuf);
  if (rf95.available()) { // check if the shield is available
    if(rf95.recv(inbuf, &lenMSG)){ // check if we got a message, if yes it stores in in our buffer
        Serial.println();
        Serial.println("Recieved a new message!");

        uint8_t message_buffer[251] = {0}; // buffer to store decoded messages into
        struct packet rec; // empty packet struct
        rec = decode(inbuf, message_buffer); // decode message into buffer and packet struct
        if(rec.destination == MY_ADDRESS){ // if the message is for us -> display it
          Serial.print("┌From: ");
          Serial.println(rec.source);
          Serial.print("├To: ");
          Serial.println(rec.destination);
          Serial.print("└Message: ");
          Serial.println((char*)message_buffer);
        }
        else{
          Serial.println("Message is not for me");
          if (!comparaison(rec.identifier, seen_messages, SEEN_MESSAGES_ARRAY_SIZE)){
            // forward message
            delay(1000); // make sure next lora has enough time to switch back to recieve mode
            rf95.send(inbuf, strlen(inbuf));  // Send message
            rf95.waitPacketSent();  // Wait until the message is sent
            Serial.println("Message forwarded!");
          }
          else
            Serial.println("I have seen and forwarded this in the past!");
        }
      }
  }
}

void get_user_input(int index, uint8_t* msgbuf){
  char c = 0;
  while(1){ // can't check for newline character, as it breaks out of the loop and ends up sending two messages 
    c = Serial.read(); // read a character from serial
    try_recieve();
    if (c != -1){ // if character is not null
      //Serial.print((int)c);
      //Serial.print(", ");

      if (c >= 32) // if character is in ascii letters and symbols -> add to message array
        msgbuf[index++] = c;

      if (c == 10) // newline character
      {
        msgbuf[index++] = 0;
        break; // go to send message
      }
    }
  }
}

void setup() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // Manual reset for lora
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  // begin serial output input
  Serial.begin(9600);
  while (!Serial); // wait for serial init

  // try lora init
  if (!rf95.init()) {
    Serial.println("LoRa initialization failed!");
    while (1); // indefinit loop to "end" program
  }
  Serial.println();
  Serial.println("LoRa initialized successfully!");
  Serial.println();

  Serial.print("LoRa is situated on network at address ");
  Serial.println(MY_ADDRESS);
  Serial.println();


  // set the send frequency of lorawan
  rf95.setFrequency(RF95_FREQ);  // Set frequency
  rf95.setTxPower(23, false);  // Set transmit power
}

void loop() {
  uint8_t msgbuf[245] = {0}; // message buffer to get input into

  if(step == STATE_ASK_ADDRESS) // ask user for address to send to
  {
    Serial.println("Please input message destination address");
    step++; // move on to next step in state machine
  }
  if(step == STATE_ASK_MESSAGE) // ask user for message to send
  {
    Serial.println("Please input message");
    step++; // move on to send step in state machine 
  }

  int index = 0;

  get_user_input(index, msgbuf); // try get user input

  
  if (step == STATE_INPUT_ADDRE){ // if wr have just inputted address
    dest = to_int(msgbuf); // get the destination address
    if (dest == -1) // if invalid, ask again
    {
      step = 0;
      Serial.println(" /!\\ Invalid address /!\\ ");
    }
    if(dest >= 0) // if valid address (redundant?)
    {
      Serial.print("Set address -> ");
      Serial.println(dest);
      step++; // move on to ask for message
    }
  }

  uint8_t outbuf[251] = {0}; // initialize a buffer for sending messages
  
  if(step == STATE_INPUT_MESSA){ // if we are askinh user to input message
    if (msgbuf[0] >= 32){ // if message starts with a valid character (is not empty)
      Serial.println();
      Serial.println((char*)msgbuf); // print out what we are sending
      struct packet send = encode_message_to_send(MY_ADDRESS, (uint8_t) dest, msgbuf, outbuf); // encode message into outbuf

      //for(int i = 0; i < 30; i++){
      //  Serial.print(outbuf[i]);
      //  Serial.print(", ");
      //}

      rf95.send(outbuf, strlen(outbuf));  // Send message
      rf95.waitPacketSent();  // Wait until the message is sent
      Serial.println("Message sent!");
      delay(1000);  // Wait 1 second before sending the next message

      step = 0; // restart state machine
    }
  }
}