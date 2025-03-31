/**
 * Small Programm to demonstrate LoRa
 * With ping and pong messages and time interval calculation.
 * Packets have ID to identify them
 * Adresses are made of two bytes 
 * More information about the protocole below here
 * 
 * Philippe.Rochat'at'eduvaud.ch
**/
// Singleton instance of the radio driver
#include <SPI.h>.     // Used to communcate with the LoRa board
#include <RH_RF95.h>  // RadioHead library to drive LoRa radio communications

// Instanciate a new RF95 drive 
RH_RF95 rf95;

///// Some globals
// Data sending
byte dataoutgoing[RH_RF95_MAX_MESSAGE_LEN];       // A buffer to prepare an outgoing message
byte lenMsg = 0;                                  // Length of that message
byte myId[2] = {10,68};                           // My adress
byte destId[2] = {0,0};                           // To store destination address
byte packetId[2];                                 // To store oacket ID
byte timeStamp[4];                                // To store message time
char *myName = "UNDEFINED ghost";                 // My Name (for RAR)
// Message reception
byte indatabuf[RH_RF95_MAX_MESSAGE_LEN];          // A buffer to store incoming messages

// MSG TYPE and protocol is of type query - reply
#define PING  1      // Query for echo
#define PONG  2      // Answer echo
#define RAR   3      // Reverse adress research
#define RARA  4      // Reverse adress research answer

/// This defines the msg format
#define MSG_POS_TYPE       0
#define MSG_POS_SENDER_H   1
#define MSG_POS_SENDER_L   2
#define MSG_POS_DEST_H     3
#define MSG_POS_DEST_L     4
#define MSG_POS_PKTID_H    5
#define MSG_POS_PKTID_L    6
#define MSG_POS_TIME_1     7
#define MSG_POS_TIME_2     8
#define MSG_POS_TIME_3     9
#define MSG_POS_TIME_4     10

// Some config
#define PING_INTERVAL 10  // Time intervall for sending a PING, rest of time is listening (and answer)
#define DEBUG_LEVEL   1   // DEBUG to enable some debugging 

// Some tools
#define MAX(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define MIN(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define LOG_TIME Serial.print((String)"["+(String)millis()+(String)"]")

// This store the current HW time in timestamp 4 bytes array
void setTimeStamp() {
  unsigned long time = millis();
#if DEBUG_LEVEL >= 3
  Serial.print("+++ Elaborating timestamp with : "+(String)time+" +++ ");
#endif
  timeStamp[3] = (byte) time;       // Lowest byte, pos 0
  timeStamp[2] = (byte)(time >> 8);  // pos 1
  timeStamp[1] = (byte)(time >> 16); // pos 2
  timeStamp[0] = (byte)(time >> 24); // pos 3
#if DEBUG_LEVEL >= 3
  Serial.print(" DETAILS : "+(String)timeStamp[0]+"."+(String)timeStamp[1]+"."+(String)timeStamp[2]+"."+(String)timeStamp[3]);
#endif  
}


unsigned long parseTimeStamp(byte buf[4]) {
  unsigned long time = 0;
  time += buf[0];
  time <<= 8;
  time += buf[1];
  time <<= 8;
  time += buf[2];
  time <<= 8;  
  time += buf[3];
  return time;
}

void buildMessage(byte type) {
  byte broadCast[2] = {0,0};
  buildMessage(type, broadCast);
}

void buildMessage(byte type, byte dest[2]) {
  byte packetId[2];
  packetId[0] = (byte)random(255);
  packetId[1] = (byte)random(255);
  buildMessage(type, dest, packetId);
}
void buildMessage(byte type, byte dest[2], byte packetId[2]) {
  buildMessage(type, dest, packetId, NULL);
}

void buildMessage(byte type, byte dest[2], byte packetId[2], char *payload) {
  dataoutgoing[MSG_POS_TYPE] = type;
  dataoutgoing[MSG_POS_SENDER_H] = myId[0];
  dataoutgoing[MSG_POS_SENDER_L] = myId[1];  
  dataoutgoing[MSG_POS_DEST_H] = destId[0];
  dataoutgoing[MSG_POS_DEST_L] = destId[1];
  dataoutgoing[MSG_POS_PKTID_H] = packetId[0];
  dataoutgoing[MSG_POS_PKTID_L] = packetId[1];


  if(type != PONG) {
    setTimeStamp();
  }
  dataoutgoing[MSG_POS_TIME_1] = timeStamp[0];
  dataoutgoing[MSG_POS_TIME_2] = timeStamp[1];
  dataoutgoing[MSG_POS_TIME_3] = timeStamp[2];
  dataoutgoing[MSG_POS_TIME_4] = timeStamp[3];
  if(payload == NULL) {
    dataoutgoing[11] = '\0';
    lenMsg = 12;
  } else {
    byte len = MIN(strlen(payload), 200);
    dataoutgoing[11] = 'len';
    char *str = &dataoutgoing[12];
    strncpy(str, payload, len);
    lenMsg = 12 + len;
  }
#if DEBUG_LEVEL >= 1
  LOG_TIME;
  Serial.print(" SENDING -- ");
  switch (type) {
    case PING:
      Serial.print("Type : PING");
      break;
    case PONG:
      Serial.print("Type : PONG");
      break;
    case RAR:
      Serial.print("Type : RAR");
      break;
    case RARA:
      Serial.print("Type : RARA");
  }
  Serial.print( " // From : " +(String)myId[0]+"."+(String)myId[1] );
  if(destId[0] == 0 && destId[1] == 0) {
    Serial.print(" // To : BROADCAST");
  } else {
    Serial.print(" // To : "+(String)destId[0]+"."+(String)destId[1]);
  }
#if DEBUG_LEVEL >= 3
  Serial.print(" DETAILS2 : "+(String)timeStamp[0]+"."+(String)timeStamp[1]+"."+(String)timeStamp[2]+"."+(String)timeStamp[3]);
#endif 
  Serial.println(" // AT ["+(String)(unsigned long int)(parseTimeStamp(&timeStamp[0]))+"]");
#endif // Level 1
}

void parseMessage(byte *buffer, uint8_t len) {
  byte type = buffer[MSG_POS_TYPE];
#if DEBUG_LEVEL >= 1
  LOG_TIME;
  Serial.print(" RECEIVING -- ");
#endif
  if( (buffer[MSG_POS_DEST_H] == myId[0] ) && (buffer[MSG_POS_DEST_L] == myId[1]) ) {
#if DEBUG_LEVEL >= 2
        Serial.println("Thats for me alone!");
#endif
  } else if((buffer[MSG_POS_DEST_H] == 0 ) && (buffer[MSG_POS_DEST_L] == 0 ) ) {
#if DEBUG_LEVEL >= 2
        Serial.println("Thats for us (bcast)");
#endif
  } else {
#if DEBUG_LEVEL >= 2
    Serial.println("None of my business ...");
#endif
    return ;
  }
  destId[0] = buffer[MSG_POS_SENDER_H]; // To reply, sender becomes dest.
  destId[1] = buffer[MSG_POS_SENDER_L];
  packetId[0] = buffer[MSG_POS_PKTID_H]; // To reply, packetId is same
  packetId[0] = buffer[MSG_POS_PKTID_L];

  byte answering = 0;

  switch(type) {
    case PING:
#if DEBUG_LEVEL >= 1
      Serial.print("Type : PING");
#endif
      break;     
    case PONG:
#if DEBUG_LEVEL >= 1
      Serial.print("Type : PONG");
#endif
      break;
  }
#if DEBUG_LEVEL >= 1
  Serial.print(" // TO : " + (String)buffer[MSG_POS_DEST_H]+"."+(String)buffer[MSG_POS_DEST_L]);
  Serial.print(" // FROM : "+ (String)buffer[MSG_POS_SENDER_H]+"."+(String)buffer[MSG_POS_SENDER_L]);
#endif
  unsigned long msgtime = 0;
  switch(type) {
    case PING:
      type = PONG;
      timeStamp[0] = buffer[MSG_POS_TIME_1];
      timeStamp[1] = buffer[MSG_POS_TIME_2];
      timeStamp[2] = buffer[MSG_POS_TIME_3];
      timeStamp[3] = buffer[MSG_POS_TIME_4];
#if DEBUG_LEVEL >= 1
      msgtime = parseTimeStamp(&buffer[MSG_POS_TIME_1]);
      Serial.println(" ==> Time in msg : ["+(String)msgtime+"]");
#endif
      buildMessage(type, destId, packetId[2]); 
      answering = 1;
      break;     
    case PONG:
      msgtime = parseTimeStamp(&buffer[MSG_POS_TIME_1]);
      Serial.print(" ==> Time in msg : ");Serial.print(msgtime);
      unsigned long delta = millis() - msgtime;
      Serial.print(" DELTA [ms]: ");Serial.println(delta);
      answering = 0;
      break;
  }
  if(answering) {
#if DEBUG_LEVEL >= 2
    Serial.print(" --> Sending reply...");
#endif
    rf95.send(dataoutgoing, sizeof(dataoutgoing));
    rf95.waitPacketSent();
  } 
#if DEBUG_LEVEL >= 2
  Serial.println(" ");
#endif 
}

void setup() {
  Serial.begin(9600);
  if (!rf95.init())
    Serial.println("init failed");
   // Defaults after init are ... MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
   // Need to change to 868.0Mhz in RH_RF95.cpp 
}

////// MAIN LOOP //////
void loop() {
  unsigned long nowTime = millis();
  unsigned long startTime = nowTime;                        // When did we start this loop
  unsigned long endLoop = startTime + 1000 * PING_INTERVAL; // WHen shall we end that iteration (and go to the next ping)

  ////////////////////////////////////
  // SENDING OUT A PING
#if DEBUG_LEVEL >=2
  Serial.println("Sending PING in the air");
#endif
  buildMessage(PING);                 // Prepare a message in dataoutgoing buffer and length in lenMsg
  rf95.send(dataoutgoing, lenMsg);    // Send it out
  rf95.waitPacketSent();              // Leave the board alone time to send

  ////////////////////////////////////
  // Waiting for incoming messages
  // Time calculation and control
  uint8_t len = sizeof(indatabuf);    // THis where we are going to collect lenght of incoming messages
  // Receiving loop
  while (nowTime < endLoop) {         // As long as we have not reached end loop time
    if (rf95.waitAvailableTimeout(endLoop - nowTime)) { // Wait for next message and at max remaining of PING_INTERVAL  
      if (rf95.recv(indatabuf, &len)) {   // Incoming message transfert to data buffer
#if DEBUG_LEVEL >= 2
        Serial.print("Got message of length : ");
        Serial.println(len);
#endif
        parseMessage(indatabuf, len);     // Read and interpret that incoming message
      } else {
        Serial.println("recv failed");  
      }
    } 
#if DEBUG_LEVEL >= 2    
    else {  // No new messages but time expired
      Serial.println("STOP listening");
    }
#endif     
    nowTime = millis();                 // Update the clock
  }
}