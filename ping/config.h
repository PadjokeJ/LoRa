#ifndef _CONFIG_H_
#define _CONFIG_H_

// Some config
#define PING_INTERVAL 60  // Time intervall in seconds for sending a PING, rest of time is listening (and answer)
#define DEBUG_LEVEL   1   // DEBUG to enable some debugging 
                          // Level 0 or no debug : programm should run silent, not waiting for serial
                          // Level 1 : log the main events like sending and receiving
                          // Level 2 : some part running
                          // Level 3 :  some details in function like calculus
#define TX_POWER 20  // From 2 to 20dBm emiter power 
#define FREQUENCY 868 // Europe is 868
byte myId[2] = {10,69};                           // My adress (do not leave 10.10)
char *myName = "UNKNOWN";                 // My Name (for RAR)

// === SELECT HARDWARE PLATFORM ===
//#define DRAGINO_SHIELD
//#define DRAGINO_DEV
#define M5STACK


#ifdef M5STACK
  #include <M5Unified.h>
  #define USE_M5_LCD
#else
  #include <SPI.h>
#endif

// === HARDWARE-SPECIFIC SETTINGS ===
#ifdef DRAGINO_SHIELD
  #define RH_MAX_MESSAGE_LEN RH_RF95_MAX_MESSAGE_LEN
#endif

#ifdef DRAGINO_DEV
  #define RH_MAX_MESSAGE_LEN RH_RF95_MAX_MESSAGE_LEN
#endif

#ifdef M5STACK
  #define RH_MAX_MESSAGE_LEN RH_SX126x_MAX_MESSAGE_LEN
#endif

#ifdef DRAGINO_SHIELD
  #define LED_SEND 7              // Leds for debugging
  #define LED_RECV 3
  #define RFM95_CS   10
  #define RFM95_RST  9
  #define RFM95_INT  2
  #define LORA_DRIVER RH_RF95
  #define LORA_HEADER "RH_RF95.h"
#endif

#ifdef DRAGINO_DEV
  #define LED_SEND 5              // Leds for debugging
  #define LED_RECV 4
  #define RFM95_CS 10
  #define RFM95_RST 9
  #define RFM95_INT 2
  #define LORA_DRIVER RH_RF95
  #define LORA_HEADER "RH_RF95.h"
#endif

#ifdef M5STACK
  #define RFM95_CS      5
  #define RFM95_RST     13
  #define RFM95_BUSY    33
  #define RFM95_DIO1    35
  #define LORA_DRIVER RH_SX126x
  #define LORA_HEADER "RH_SX126x.h"
#endif

// Some definition below ... not to be twisted

// === MODEM CONFIGURATION ===
// These macros define LoRa modulation parameters like bandwidth, spreading factor, and coding rate.
// They are defined per platform (RF95 vs SX126x) because the enums differ.

// --- M5STACK: SX126x driver ---
#ifdef M5STACK
  // Choix valides pour RH_SX126x
  #define MODEM_CONFIG1 RH_SX126x::LoRa_Bw125Cr45Sf128    // Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range.
  #define MODEM_CONFIG2 RH_SX126x::LoRa_Bw500Cr45Sf128    // Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range.
  #define MODEM_CONFIG3 RH_SX126x::LoRa_Bw31_25Cr48Sf512  // Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range.
  #define MODEM_CONFIG4 RH_SX126x::LoRa_Bw125Cr48Sf4096   // Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, low data rate, CRC on. Slow+long range.
  #define MODEM_CONFIG5 RH_SX126x::LoRa_Bw125Cr45Sf2048   // Bw = 125 kHz, Cr = 4/5, Sf = 2048chips/symbol, CRC on. Slow+long range.
  #define MODEM_CONFIG6 MODEM_CONFIG3  // celui qui vise "long range"
#else
// --- DRAGINO / UNO: RH_RF95 driver ---
  #define MODEM_CONFIG1 RH_RF95::Bw125Cr45Sf128
  #define MODEM_CONFIG2 RH_RF95::Bw500Cr45Sf128
  #define MODEM_CONFIG3 RH_RF95::Bw31_25Cr48Sf512
  #define MODEM_CONFIG4 RH_RF95::Bw125Cr48Sf4096
  #define MODEM_CONFIG5 RH_RF95::Bw125Cr45Sf2048
  #define MODEM_CONFIG6 RH_RF95::Bw31_25Cr48Sf512
#endif

// Set the active configuration here (default: long range)
#define MODEM_CONFIG MODEM_CONFIG6

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

// === LOGGING MACROS ===
#ifdef M5STACK
  #define LOG_DEVICE M5.Display
#else
  #define LOG_DEVICE Serial
#endif


#if DEBUG_LEVEL > 0
//  #define INIT_SERIAL() Serial.begin(9600)
  #define INIT_SERIAL() do { Serial.begin(9600); while (!Serial) { delay(10); } } while(0)
#else
  #define INIT_SERIAL()  // rien
#endif

#if DEBUG_LEVEL >= 1
  #define LOG1(x)     LOG_DEVICE.print(x)
  #define LOGLN1(x)   LOG_DEVICE.println(x)
#else
  #define LOG1(x)
  #define LOGLN1(x)
#endif

#if DEBUG_LEVEL >= 2
  #define LOG2(x)     LOG_DEVICE.print(x)
  #define LOGLN2(x)   LOG_DEVICE.println(x)
#else
  #define LOG2(x)
  #define LOGLN2(x)
#endif

#if DEBUG_LEVEL >= 3
  #define LOG3(x)     LOG_DEVICE.print(x)
  #define LOGLN3(x)   LOG_DEVICE.println(x)
#else
  #define LOG3(x)
  #define LOGLN3(x)
#endif

// Log horodaté (sécurisé dans les blocs conditionnels)
#if DEBUG_LEVEL >= 1
  #define LOG_TIME() do { LOG_DEVICE.print("["); LOG_DEVICE.print(millis()); LOG_DEVICE.print("] "); } while(0)
#else
  #define LOG_TIME()
#endif



#endif // _CONFIG_H_



 	

 	


 	

