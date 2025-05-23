#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>
#include <RH_RF95.h>

#define MY_ADDRESS (uint8_t)9

#define LISTEN_TIME 10 //in seconds

#define RFM95_CS   10
#define RFM95_RST  9
#define RFM95_INT  2

#define MODEM_CONFIG RH_RF95::Bw31_25Cr48Sf512

#define LORA_STRENGTH 23
#define LORA_SS 10
#define LORA_FREQ 868 //MHz

#define COMPARISON_LIST_SIZE 16

#define MAX_BYTES_LEN RH_RF95_MAX_MESSAGE_LEN
#define MAX_MESSAGE_LEN (MAX_BYTES_LEN - 6)

#define MESSAGE_SEEN_CODE 0
#define MESSAGE_TO_ME_CODE 1
#define MESSAGE_TO_FORWARD_CODE 2

#define RECIEVE_ERROR_SUCCESS 0
#define RECIEVE_ERROR_NO_MESSAGE 1
#define RECIEVE_ERROR_FAILED 2
// states : STATE_ASK_ADDRESS, STATE_INPUT_ADDRE, STATE_ASK_MESSAGE, STATE_INPUT_MESSA
#define STATE_ASK_ADDRESS 0
#define STATE_INPUT_ADDRE 1
#define STATE_ASK_MESSAGE 2
#define STATE_INPUT_MESSA 3


#endif