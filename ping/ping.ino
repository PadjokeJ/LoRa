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
#define TX_POWER 13.  // From 2 to 20dBm emiter power 
#define FREQUENCY 868 // Europe is 968
///// Some globals
// Data sending
byte dataoutgoing[RH_RF95_MAX_MESSAGE_LEN];       // A buffer to prepare an outgoing message
byte lenMsg = 0;                                  // Length of that message
byte myId[2] = {10,10};                           // My adress (do not leave 10.10)
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

/// This defines the msg format (see doc)
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
#define MSG_POS_PDLEN      11

// Some config
#define PING_INTERVAL 10  // Time intervall for sending a PING, rest of time is listening (and answer)
#define DEBUG_LEVEL   1   // DEBUG to enable some debugging 
                          // Level 0 or no debug : programm should run silent
                          // Level 1 : log the main events like sending and receiving
                          // Level 2 : some part running
                          // Level 3 :  some details in function like calculus

// Some tools
#define MAX(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define MIN(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define LOG_TIME Serial.print((String)"["+(String)millis()+(String)"]")

// This store the current HW time in timestamp global 4 bytes array
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

// Get the time from a 4 bytes array and put it down into a returned value
unsigned long parseTimeStamp(byte buf[4]) {
  unsigned long time = 0;
  time += buf[0];   // Highest level byte
  time <<= 8;       // And then shift it to the left
  time += buf[1];   // Etc ...
  time <<= 8;
  time += buf[2];
  time <<= 8;  
  time += buf[3];
  return time;
}

// Build a random two bytes packet id
void setPacketId() {
  packetId[0] = (byte)random(255);
  packetId[1] = (byte)random(255);  
}

////////////////////////////////////////////////////////////////
// buildMessage build a message stored into dataoutgoing buffer
// and store len of built message into lenMsg. We have used 
// Overload technique here to have default params python like style
void buildMessage(byte type) {  // Specify only type
  byte broadCast[2] = {0,0};    // Default address is set to broadcast
  buildMessage(type, broadCast);
}

void buildMessage(byte type, byte dest[2]) { // ... and specify address
  setPacketId();        
  buildMessage(type, dest, NULL); 
}

// And specify a payload.
// This is the real one with all the code
void buildMessage(byte type, byte dest[2], char *payload) {
  // Build the header either from params or stored in globals
  dataoutgoing[MSG_POS_TYPE] = type;
  dataoutgoing[MSG_POS_SENDER_H] = myId[0];
  dataoutgoing[MSG_POS_SENDER_L] = myId[1];  
  dataoutgoing[MSG_POS_DEST_H] = destId[0];
  dataoutgoing[MSG_POS_DEST_L] = destId[1];
  
  if((type == PING) || (type == RAR)) {    // If this is not an answer
    setTimeStamp();
    setPacketId();
  } // Else it should habve been set  in the request
  dataoutgoing[MSG_POS_PKTID_H] = packetId[0];
  dataoutgoing[MSG_POS_PKTID_L] = packetId[1];
  dataoutgoing[MSG_POS_TIME_1] = timeStamp[0];
  dataoutgoing[MSG_POS_TIME_2] = timeStamp[1];
  dataoutgoing[MSG_POS_TIME_3] = timeStamp[2];
  dataoutgoing[MSG_POS_TIME_4] = timeStamp[3];
  if(payload == NULL) {
    dataoutgoing[11] = '\0';
    lenMsg = 12;                            // End of header of 12 bytes
  } else {
    byte len = MIN(strlen(payload), 200);
    dataoutgoing[11] = 'len';               // End of header
    char *str = &dataoutgoing[12];          // Enventually add a payload
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

////////////////////////////////////////////////////////////////
// parseMessage is to paarse incoming message
void parseMessage(byte *buffer, uint8_t len) {
  byte type = buffer[MSG_POS_TYPE];       // First extract message type
#if DEBUG_LEVEL >= 1
  LOG_TIME;
  Serial.print(" RECEIVING -- ");
#endif
  if( (buffer[MSG_POS_DEST_H] == myId[0] ) && (buffer[MSG_POS_DEST_L] == myId[1]) ) { // Check if it's for me
#if DEBUG_LEVEL >= 2
        Serial.println("Thats for me alone!");
#endif
  } else if((buffer[MSG_POS_DEST_H] == 0 ) && (buffer[MSG_POS_DEST_L] == 0 ) ) {      // or for everyone (bcast)
#if DEBUG_LEVEL >= 2
        Serial.println("Thats for us (bcast)");
#endif
  } else {                                                                            // Or not, skip
#if DEBUG_LEVEL >= 2
    Serial.println("None of my business ...");
#endif
    return ;
  }
  // Extract then adresses and packetID
  destId[0] = buffer[MSG_POS_SENDER_H]; // To reply, sender becomes dest.
  destId[1] = buffer[MSG_POS_SENDER_L];
  packetId[0] = buffer[MSG_POS_PKTID_H]; // To reply, packetId is same
  packetId[0] = buffer[MSG_POS_PKTID_L];
#if DEBUG_LEVEL >= 1
  switch(type) {
    case PING:
      Serial.print("Type : PING");
      break;     
    case PONG:
      Serial.print("Type : PONG");
      break;
  }
  Serial.print(" // TO : " + (String)buffer[MSG_POS_DEST_H]+"."+(String)buffer[MSG_POS_DEST_L]);
  Serial.print(" // FROM : "+ (String)buffer[MSG_POS_SENDER_H]+"."+(String)buffer[MSG_POS_SENDER_L]);
#endif
  switch(type) {
    case PING:                               // Ping message
      timeStamp[0] = buffer[MSG_POS_TIME_1]; // Retrive 4 bytes of timestamp
      timeStamp[1] = buffer[MSG_POS_TIME_2];
      timeStamp[2] = buffer[MSG_POS_TIME_3];
      timeStamp[3] = buffer[MSG_POS_TIME_4];
      packetId[0]  = buffer[MSG_POS_PKTID_H]; // Retrive 2 bytes of pycketId
      packetId[1]  = buffer[MSG_POS_PKTID_L];
#if DEBUG_LEVEL >= 1
      unsigned long msgtime = 0;
      msgtime = parseTimeStamp(&buffer[MSG_POS_TIME_1]);
      Serial.println(" ==> Time in msg : ["+(String)msgtime+"]");
#endif
      buildMessage(PONG, destId); 
      break;     
    case PONG:
      msgtime = parseTimeStamp(&buffer[MSG_POS_TIME_1]);  // Get the timestamp, calculate epoch time
#if DEBUG_LEVEL >= 1
      Serial.print(" ==> Time in msg : ");Serial.print(msgtime); 
#endif
      unsigned long delta = millis() - msgtime;           // And diff to now to get the ping time
#if DEBUG_LEVEL >= 1
      Serial.print(" DELTA [ms]: ");Serial.println(delta);
#endif
      break;
    case RAR:
      packetId[0]  = buffer[MSG_POS_PKTID_H]; // Retrive 2 bytes of pycketId
      packetId[1]  = buffer[MSG_POS_PKTID_L];
      buildMessage(RARA, destId, myName); 
      break;
  }
  if((type==PING) || (type=RAR)) {    // We have to send a reply to PING and RAR
#if DEBUG_LEVEL >= 2
    Serial.print(" --> Sending reply...");
#endif
    rf95.send(dataoutgoing, sizeof(dataoutgoing));
    rf95.waitPacketSent();
  } 
#if DEBUG_LEVEL >= 1
  Serial.println(" ");
#endif 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Main Programm with setup() and loop()

void setup() {
  Serial.begin(9600);     // Serial setup
  Serial.println("Starting ping/pong");
  rf95.setTxPower(TX_POWER);    
  rf95.setModemConfig(RH_RF95::Bw125Cr45Sf2048);
  rf95.setFrequency(FREQUENCY);

  if (!rf95.init())       // Radio init
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