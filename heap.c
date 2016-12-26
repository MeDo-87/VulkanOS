#include "heap.h"

extern UInt32 end;
static UInt32 placementAddress = (UInt32)&end;

void* malloc(UInt32 size, UInt32 aligned , UInt32* phys)
{
	if(aligned ==1 && (placementAddress & 0xFFFFFF00))
	{
		placementAddress &= 0xFFFFF000;
		placementAddress += 0x1000;
	}
	if(phys)
	{
		*phys = placementAddress;
	}
	UInt32 tmp = placementAddress;
	placementAddress += size;
	return reinterpret_cast<void*>(tmp);
	
}
void* kmalloc(UInt32 size) //Just allocate a required memory
{
	return malloc(size,0,0);
}
void* kmallocA(UInt32 size) //Allocate page aligned memory
{
	return malloc(size,1,0);
}
void* kmallocP(UInt32 size, UInt32* physicalAddress) //allocate memory and provide physical as well as virtual address
{
	return malloc(size,0,physicalAddress);
}

void* kmallocAP(UInt32 size, UInt32* physicalAddress) //alligned memory with physical address
{
	return malloc(size,1,physicalAddress);
}


