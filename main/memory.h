/*
Memory code to store messages id's and prevent infinite loops
Made by Maurits van der Net and Agnès Briens

*/
#include <stdint.h>

#ifndef MEMORY_H
#define MEMORY_H
int comparaison(uint16_t id, uint16_t* knownlist, int taille);
#endif
