#ifndef HEAP_H_
#define HEAP_H_

#include "common.h"

void* kmalloc(UInt32 size); //Just allocate a required memory
void* kmallocA(UInt32 size); //Allocate page aligned memory

void* kmallocP(UInt32 size, UInt32* physicalAddress); //allocate memory and provide physical as well as virtual address

void* kmallocAP(UInt32 size, UInt32* physicalAddress); //alligned memory with physical address


#endif
