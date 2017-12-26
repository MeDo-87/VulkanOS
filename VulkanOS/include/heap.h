#ifndef HEAP_H_
#define HEAP_H_

#include "common.h"


namespace Allocator
{
void* kmalloc(UInt32 size); //Just allocate a required memory
void* kmallocA(UInt32 size); //Allocate page aligned memory

void* kmalloc(UInt32 size, UInt32* physicalAddress); //allocate memory and provide physical as well as virtual address

void* kmallocA(UInt32 size, UInt32* physicalAddress); //alligned memory with physical address

void* malloc(UInt32 size, UInt32 aligned, UInt32* phys);
}
#endif
