#include "heap.h"
#include "Utils.h"

using namespace Allocator;
extern UInt32 end;
static UInt32 placementAddress = (UInt32)&end;

void* Allocator::malloc(UInt32 size, UInt32 aligned , UInt32* phys)
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
void* Allocator::kmalloc(UInt32 size) //Just allocate a required memory
{
	return malloc(size,0,0);
}
void* Allocator::kmallocA(UInt32 size) //Allocate page aligned memory
{
	return malloc(size,1,0);
}
void* Allocator::kmalloc(UInt32 size, UInt32* physicalAddress) //allocate memory and provide physical as well as virtual address
{
	return malloc(size,0,physicalAddress);
}

void* Allocator::kmallocA(UInt32 size, UInt32* physicalAddress) //alligned memory with physical address
{
	return malloc(size,1,physicalAddress);
}

