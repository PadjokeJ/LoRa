#ifndef RECIEVE_H
#define RECIEVE_H

#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

class Receive {
  public:
    Receive(int ssPin, int rstPin, int freq);
    void init();
    void receiveMessage();

  private:
    RH_RF95 rf95;
    int _ssPin;
    int _rstPin;
    int _freq;
};

#endif