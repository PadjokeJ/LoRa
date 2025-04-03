#ifndef RECEIVE_H
#define RECEIVE_H

#include <Arduino.h>
#include <RH_RF95.h>
#include "lorainit.h"

class Receive {
  public:
    Receive(Lorainit &loraModule);
    void startReceive();
    void receiveMessage();

  private:
   Lorainit &lora; // Reference to the Lorainit object
  
};

#endif