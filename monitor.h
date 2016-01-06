#ifndef MONITOR_H_
#define MONITOR_H_
#include "common.h"



//Write a single character out to the screen
void putc(char c);

//Clear the screen
void clear();

//output a null-terminated ASCII string
void putString(char *c);

void WriteHex(UInt32 n);

void WriteDec(UInt32 n);

#endif
