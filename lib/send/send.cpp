#include "send.h"

Send::Send(Lorainit &loraModule) : lora(loraModule) {}

void Send::startSend() {
    lora.lora.setTxPower(21, false); // Set transmit power (in dBm)
    lora.lora.setModeTx(); // Set mode to transmit
    Serial.println("Ready to send");
}

void Send::sendPackets() {                       

/* -----------------------Là Romain et Jules vous mettez votre code qui a à l'interieur du loop-------------------------------------------*/

    Serial.println("Sending: MessageToutCool-OC-INFO");

    uint8_t message[] = "MessageToutCool-OC-INFO";
    lora.lora.send(message, sizeof(message));

    lora.lora.waitPacketSent();

}