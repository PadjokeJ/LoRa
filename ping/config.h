#ifndef _CONFIG_H_
#define _CONFIG_H_

// Some config
#define PING_INTERVAL 10  // Time intervall in seconds for sending a PING, rest of time is listening (and answer)
#define DEBUG_LEVEL   1   // DEBUG to enable some debugging 
                          // Level 0 or no debug : programm should run silent, not waiting for serial
                          // Level 1 : log the main events like sending and receiving
                          // Level 2 : some part running
                          // Level 3 :  some details in function like calculus
#define TX_POWER 20  // From 2 to 20dBm emiter power 
#define FREQUENCY 868 // Europe is 868

#define MODEM_CONFIG1 Bw125Cr45Sf128   // Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range.
#define MODEM_CONFIG2 Bw500Cr45Sf128   // Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range.
#define MODEM_CONFIG3 Bw31_25Cr48Sf512 // Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range.
#define MODEM_CONFIG4 Bw125Cr48Sf4096  // Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, low data rate, CRC on. Slow+long range.
#define MODEM_CONFIG5 Bw125Cr45Sf2048  // Bw = 125 kHz, Cr = 4/5, Sf = 2048chips/symbol, CRC on. Slow+long range.

#define MODEM_CONFIG MODEM_CONFIG4

// Global vars
byte myId[2] = {10,10};                           // My adress (do not leave 10.10)
char *myName = "MARCELIN33";                 // My Name (for RAR)

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
// Some tools
#define MAX(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define MIN(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define LOG_TIME Serial.print((String)"["+(String)millis()+(String)"]")

#endif // _CONFIG_H_



 	

 	


 	

 	
