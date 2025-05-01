#ifndef SEND_H
#define SEND_H

#include <Arduino.h>
#include <stdint.h>
#include <RH_RF95.h>
#include "lorainit.h"
#include "../../config.h"

class Send {
    public:
        Send(Lorainit &loraModule);
        void startSend();
        void sendPackets(uint8_t* packet);
        
    private:
        Lorainit &lora;
};


#endif