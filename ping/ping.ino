/**
 * Small Programm to demonstrate LoRa
 * With ping and pong messages and time interval calculation.
 * Packets have ID to identify them
 * Adresses are made of two bytes 
 * More information about the protocole below here
 * Compatible with DRAGINO or M5STACK (SX1276 or SX126x)
 * 
 * Philippe.Rochat'at'eduvaud.ch
**/
// Singleton instance of the radio driver
#include <SPI.h>     // Used to communcate with the LoRa board
#include "config.h"

#include LORA_HEADER  // Defined in config.h

// Instanciate a new RF95 drive 
// Instantiate the appropriate LoRa driver
//LORA_DRIVER rf95(RFM95_CS, RFM95_INT);
#ifdef M5STACK
RH_SX126x loraDriver(RFM95_CS, RFM95_DIO1, RFM95_RST, RFM95_BUSY);
#else
LORA_DRIVER loraDriver(RFM95_CS, RFM95_INT);
#endif

//RH_RF95 rf95(RFM95_CS, RFM95_INT);

//RH_RF95 rf95;
///// Some globals
// Data sending
byte dataoutgoing[RH_MAX_MESSAGE_LEN];       // A buffer to prepare an outgoing message
byte lenMsg = 0;                                  // Length of that message
const byte broadCast[2] = {0,0};                  // Constant value for broadcast
byte packetId[2];                                 // To store oacket ID
byte timeStamp[4];                                // To store message time
// Message reception
byte indatabuf[RH_MAX_MESSAGE_LEN];          // A buffer to store incoming messages
unsigned long interval = (unsigned long)1000 * PING_INTERVAL;

// This store the current HW time in timestamp global 4 bytes array
void setTimeStamp() {
  unsigned long time = millis();
  timeStamp[3] = (byte) time;       // Lowest byte, pos 0
  timeStamp[2] = (byte)(time >> 8);  // pos 1
  timeStamp[1] = (byte)(time >> 16); // pos 2
  timeStamp[0] = (byte)(time >> 24); // pos 3
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

void logHeaderInfo(byte* buf) {
#if DEBUG_LEVEL >= 1
  switch (buf[MSG_POS_TYPE]) {
    case PING:
      LOG1(" <<Type : PING>> ");
      break;
    case PONG:
      LOG1(" <<Type : PONG>> ");
      break;
    case RAR:
      LOG1(" <<Type : RAR>> ");
      break;
    case RARA:
      LOG1(" <<Type : RARA>> ");
      break;
    default:
      LOG1(" <<Type : UNKNOWN>> ");
    break;
  }
  LOG1("<<FROM : "); LOG1((String)buf[MSG_POS_SENDER_H]+"."+String(buf[MSG_POS_SENDER_L]));
  LOG1(" // TO : "); LOG1((String)buf[MSG_POS_DEST_H]+"."+String(buf[MSG_POS_DEST_L])); LOG1(">>");
#endif
}

void logFrame(byte* buf, int len) {
#if DEBUG_LEVEL >= 3
  LOG3("Trame HEX : ");
  for (int i = 0; i < len; i++) {
    if (buf[i] < 16) Serial.print("0");
    Serial.print(buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
#endif
}

////////////////////////////////////////////////////////////////
// buildMessage build a message stored into dataoutgoing buffer
// and store len of built message into lenMsg. We have used 
// Overload technique here to have default params python like style
void buildMessage(byte type) {  // Specify only type
  buildMessage(type, &broadCast[0]);
}

void buildMessage(byte type, const byte dest[2]) { // ... and specify address
  setPacketId();        
  buildMessage(type, dest, NULL); 
}

// And specify a payload.
// This is the real one with all the code
void buildMessage(byte type, const byte dest[2], char *payload) {
  // Build the header either from params or stored in globals
  LOG_TIME();
  LOG1(" SENDING -- ");
  dataoutgoing[MSG_POS_TYPE] = type;
  dataoutgoing[MSG_POS_SENDER_H] = myId[0];
  dataoutgoing[MSG_POS_SENDER_L] = myId[1];  
  dataoutgoing[MSG_POS_DEST_H] = dest[0];
  dataoutgoing[MSG_POS_DEST_L] = dest[1];
  
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
    dataoutgoing[11] = len;               // End of header
    char *str = (char*)&dataoutgoing[12];          // Eventually add a payload
    strncpy(str, payload, len);
    lenMsg = 12 + len;
  }
// Some debug display next
logHeaderInfo(dataoutgoing);
LOGLN1("");
} // End of build message

////////////////////////////////////////////////////////////////
// parseMessage is to parse incoming message
void parseMessage(byte *buffer, uint8_t len) {
  byte type = buffer[MSG_POS_TYPE];       // First extract message type
  LOG_TIME();
  LOG1(" RECEIVING -- ");

  if( (buffer[MSG_POS_DEST_H] == myId[0] ) && (buffer[MSG_POS_DEST_L] == myId[1]) ) { // Check if it's for me
    LOG2(" -- Thats for me alone! -- ");
  } else if((buffer[MSG_POS_DEST_H] == 0 ) && (buffer[MSG_POS_DEST_L] == 0 ) ) {      // or for everyone (bcast)
    LOG2(" -- Thats for us (bcast) -- ");
  } else {     
    LOG2(" -- None of my business -- ");                                                                       // Or not, skip
    return ;
  }
  // Extract then addresses and packetID
  byte destId[2];
  destId[0] = buffer[MSG_POS_SENDER_H]; // To reply, sender becomes dest.
  destId[1] = buffer[MSG_POS_SENDER_L];
  packetId[0] = buffer[MSG_POS_PKTID_H]; // To reply, packetId is same
  packetId[1] = buffer[MSG_POS_PKTID_L];
  
  logHeaderInfo(buffer);
  unsigned long msgTime = 0;
  switch(type) {
    case PING:                               // Ping message
      timeStamp[0] = buffer[MSG_POS_TIME_1]; // Retrive 4 bytes of timestamp
      timeStamp[1] = buffer[MSG_POS_TIME_2];
      timeStamp[2] = buffer[MSG_POS_TIME_3];
      timeStamp[3] = buffer[MSG_POS_TIME_4];
      packetId[0]  = buffer[MSG_POS_PKTID_H]; // Retrive 2 bytes of packetId
      packetId[1]  = buffer[MSG_POS_PKTID_L];
      msgTime = parseTimeStamp(&buffer[MSG_POS_TIME_1]);  // Get the timestamp, calculate epoch time
      LOGLN1(" ==> Time in msg : ["+(String)msgTime+"]");
      buildMessage(PONG, destId); 
      break;     
    case PONG: {
      msgTime = parseTimeStamp(&buffer[MSG_POS_TIME_1]);  // Get the timestamp, calculate epoch time
      LOG1(" ==> Time in msg : ["+(String)msgTime+"]");
      unsigned long delta = millis() - msgTime;           // And diff to now to get the ping time
      LOG1("DELTA [ms]: "); LOG1(delta);
      break;
    }
    case RAR:
      packetId[0]  = buffer[MSG_POS_PKTID_H]; // Retrive 2 bytes of pycketId
      packetId[1]  = buffer[MSG_POS_PKTID_L];
      buildMessage(RARA, destId, myName); 
      break;
    case RARA:
#if DEBUG_LEVEL >= 1
      // Should display address resolved to ...
#endif
      break;
  }
  if((type==PING) || (type==RAR)) {    // We have to send a reply to PING and RAR
    LOG2(" --> Should send a reply ");
    blinkSend();                        // We blink before to avoir delay after sent and be ready for reception ASAP
    loraDriver.send(dataoutgoing, lenMsg);
    loraDriver.waitPacketSent();
  } 
  LOGLN1("");
}

void resetShield() {
  // Reset LoRa module (facultatif, mais souvent utile)
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
}

void blinkSend() {
#ifndef M5STACK
  digitalWrite(LED_SEND, HIGH);
  delay(50);
  digitalWrite(LED_SEND, LOW);
#endif
}

void blinkRecv() {
#ifndef M5STACK
  digitalWrite(LED_RECV, HIGH);
  delay(50);
  digitalWrite(LED_RECV, LOW);
#endif
}

void progressBlink() {
  blinkSend();
  delay(100);
  blinkRecv();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Main Programm with setup() and loop()
void setup() {
#ifndef M5STACK
  pinMode(LED_SEND, OUTPUT);      // First of all, low level comm
  pinMode(LED_RECV, OUTPUT);
#endif
  progressBlink();
  INIT_SERIAL();                  // Second level logging through serial (if level > 0)
  progressBlink();
  delay(100);                     // Security delay
#ifdef M5STACK
  auto cfg = M5.config();
  M5.begin(cfg);
  delay(100);
  M5.Display.setTextSize(2);  // Pour plus de lisibilité
  M5.Display.setCursor(0, 0);
  M5.Display.clear();         // Efface l’écran au démarrage
  M5.Display.print("Booting...");
#endif
  progressBlink();
#ifdef M5STACK
  M5.Display.print("+");
  SPI.begin(18, 19, 23);          // SCK, MISO, MOSI pour ESP32
  M5.Display.print("+");
#endif
  resetShield();                  // Reset lora shield to avoid Serial depedency
  LOG1(">>> Starting"); 
  if (!loraDriver.init()) {                 // Radio init
    LOGLN1("LORA INITIALISATION FAILED !!!");
    Serial.println("init failed");    // Not LOG, since there is not level for that
    while(true) {                     // Endless blinking loop if radio didn't init
      blinkRecv();
    }
  }
  LOGLN1("*");
  progressBlink();
  loraDriver.setFrequency(FREQUENCY);              
  LOG1("Frequency set to: "); LOG1(FREQUENCY); LOGLN1(" MHz");
  loraDriver.setModemConfig(MODEM_CONFIG);

  loraDriver.setTxPower(TX_POWER);                   // Set the output power
  LOG1("Power set to: "); LOG1(TX_POWER); LOGLN1(" MHz");
  progressBlink();
  // End of setup : both leds UP for 500ms
  Serial.println("<<<< Started");
  #ifndef M5STACK
  digitalWrite(LED_RECV, HIGH);
  digitalWrite(LED_SEND, HIGH);
  delay(500);
  digitalWrite(LED_RECV, LOW);
  digitalWrite(LED_SEND, LOW);
  delay(500);                       // To make it clear from other following blinkings
  #endif
}

////// MAIN LOOP //////
void loop() {
  unsigned long nowTime = millis();
  unsigned long startTime = nowTime;                        // When did we start this loop
  unsigned long endLoop = startTime + interval; // WHen shall we end that iteration (and go to the next ping)

  ////////////////////////////////////
  // SENDING OUT A PING
  LOG2("Sending PING in the air");
  buildMessage(PING);                 // Prepare a message in dataoutgoing buffer and length in lenMsg
  blinkSend();                        // We blink before to avoir delay after sent and be ready for reception ASAP
  loraDriver.send(dataoutgoing, lenMsg);    // Send it out
  loraDriver.waitPacketSent();              // Leave the board alone time to send

  ////////////////////////////////////
  // Waiting for incoming messages
  // Time calculation and control
  uint8_t len = sizeof(indatabuf);          // THis where we are going to collect lenght of incoming messages
  // Receiving loop
  while (nowTime < endLoop) {               // As long as we have not reached end loop time
    if (loraDriver.waitAvailableTimeout(endLoop - nowTime)) { // Wait for next message and at max remaining of PING_INTERVAL  
      if (loraDriver.recv(indatabuf, &len)) {     // Incoming message transfert to data buffer
        blinkRecv();
        LOG2("Got message of length : "); LOGLN2(len);
        parseMessage(indatabuf, len);     // Read and interpret that incoming message
        logFrame(indatabuf,len);          // Log details at level 3
      } else {
        LOGLN1("ERR - Received failed");
      }
    } 
    else {  // No new messages but time expired  
      LOGLN2("STOP listening for next ping");  
    }
    nowTime = millis();                 // Update the clock
  }
}