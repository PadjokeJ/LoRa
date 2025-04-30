#ifndef SERIAL_H
#define SERIAL_H

#include <cppQueue.h>
#include <stdint.h>
#include <SPI.h>

#include <stdint.h>

void SerialInput(cppQueue queue);

uint8_t* SerialOutput(cppQueue queue);

#endif