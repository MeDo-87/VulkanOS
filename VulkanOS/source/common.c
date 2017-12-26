#include "common.h"
//I/O
UInt16 ReadWord(UInt16 port)
{
	UInt16 ret;
	asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}



//Memory functions
void memcpy(void* src, void* dest, UInt32 length)
{
	
	if(src == 0 || dest == 0 || length < 1)
	{
		return;
	}
	
	for(unsigned int i = 0; i < length; i++)
	{
		((UInt8*)dest)[i]= ((UInt8*)src)[i];
	}
	return;
}
void memset(void* dest, UInt8 data, UInt32 length)
{
	
	if(dest == 0 || length < 1)
	{
		return;
	}
	for(unsigned int i = 0; i < length; i++)
	{
		((UInt8*)dest)[i] = data;
	}
	return;
}
