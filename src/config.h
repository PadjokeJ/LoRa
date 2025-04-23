#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

uint8_t my_address = 0; //change this

#define RFM95_CS   10
#define RFM95_RST  9
#define RFM95_INT  2

#define MODEM_CONFIG RH_RF95::Bw31_25Cr48Sf512

#define LORA_STRENGTH 20
#define LORA_FREQ 868 //MHz

#define COMPARISON_LIST_SIZE 16
uint8_t comparison_list[COMPARISON_LIST_SIZE] = {0};

#define MAX_BYTES_LEN RH_RF95_MAX_MESSAGE_LEN
#define MAX_MESSAGE_LEN (RH_RF95_MAX_MESSAGE_LEN - 6)

#define MESSAGE_SEEN_CODE 0
#define MESSAGE_TO_ME_CODE 1
#define MESSAGE_TO_FORWARD_CODE 2

#endif