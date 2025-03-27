#include "recieve.h"
/*
  Simple Queue
  Simple queue demonstration
  
  LIFO / FIFO implementations can be tested by changing IMPLEMENTATION

  This example code is in the public domain.

  created 22 March 2017
  modified 04 November 2020
  by SMFSW
 */

#include <cppQueue.h>

#define	IMPLEMENTATION	LIFO


typedef struct strRec {
	uint16_t	entry1;
	uint16_t	entry2;
} Rec;

byte buffer[100] = {1,2,3};


cppQueue	q(300);	// Instantiate queue

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
  q.push(buffer);
  Serial.println(q.pop(buffer));
}

// the loop function runs over and over again forever
void loop() {
	
}