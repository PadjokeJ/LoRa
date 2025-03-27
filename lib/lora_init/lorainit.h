#ifndef LORAINIT_H
#define LORAINIT_H

#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

class Lorainit {
  public:
    Lorainit(int ssPin, int rstPin, int freq);
    void init();

  private:
    RH_RF95 rf95;
    int _ssPin;
    int _rstPin;
    int _freq;
};

#endif