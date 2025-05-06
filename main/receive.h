#ifndef RECEIVE_H
#define RECEIVE_H

#include <Arduino.h>
#include <RH_RF95.h>
#include "lorainit.h"

#include "config.h"

class Receive {
  public:
    Receive(Lorainit &loraModule);
    void startReceive();
    uint8_t receiveMessage(uint8_t* message_buffer, uint8_t max_len);

  private:
    Lorainit &lora;
};

#endif