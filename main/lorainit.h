#ifndef LORAINIT_H
#define LORAINIT_H

#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

class Lorainit {
  public:
    Lorainit(int ssPin, int freq, int intPin);
    void init();  // Instance publique pour Romain et Sacha
    RH_RF95& lora();

  private:
    int _ssPin;
    int _freq;
    RH_RF95* _rf95;
};

#endif
