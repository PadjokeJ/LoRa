#ifndef SERIAL_H
#define SERIAL_H

#include <cppQueue.h>
#include <stdint.h>
#include <SPI.h>

#include <stdint.h>

void SerialInput(cppQueue queue);

void SerialOutput(cppQueue queue, char* array_buffer);

#endif