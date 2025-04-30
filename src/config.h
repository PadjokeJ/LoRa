#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <RH_RF95.h>

uint8_t my_address = 0; //change this
#define LISTEN_TIME 10 //in seconds

#define RFM95_CS   10
#define RFM95_RST  9
#define RFM95_INT  2

#define MODEM_CONFIG RH_RF95::Bw31_25Cr48Sf512

#define LORA_STRENGTH 20
#define LORA_SS 10
#define LORA_FREQ 868 //MHz

#define COMPARISON_LIST_SIZE 16
uint8_t comparison_list[COMPARISON_LIST_SIZE] = {0};

#define MAX_BYTES_LEN RH_RF95_MAX_MESSAGE_LEN
#define MAX_MESSAGE_LEN (MAX_BYTES_LEN - 6)

#define MESSAGE_SEEN_CODE 0
#define MESSAGE_TO_ME_CODE 1
#define MESSAGE_TO_FORWARD_CODE 2

#define RECIEVE_ERROR_SUCCESS 0
#define RECIEVE_ERROR_NO_MESSAGE 1
#define RECIEVE_ERROR_FAILED 2


#endif