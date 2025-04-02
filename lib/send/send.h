#ifndef SEND_H
#define SEND_H

#include <Arduino.h>
#include <RH_RF95.h>
#include "lorainit.h"

class Send {
    public:
        Send(Lorainit &loraModule);
        void startSend();
        void sendPackets();
        
    private:
        Lorainit &lora;
};


#endif