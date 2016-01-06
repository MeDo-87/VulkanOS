#include "common.h"
//I/O
inline void WriteByte(UInt16 port, UInt8 value)
{
	asm volatile("outb %1, %0" : : "dN"(port), "a" (value));

}
inline UInt8 ReadByte(UInt16 port)
{
	UInt8 ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}
UInt16 ReadWord(UInt16 port)
{
	UInt16 ret;
	asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

//Memory functions
void memcpy(void* src, void* dest, UInt32 length)
{
	int i;	
	if(src == 0 || dest == 0 || length < 1)
	{
		return;
	}
	
	for(i = 0; i < length; i++, src++, dest++)
	{
		*((UInt8*)dest) = *((UInt8*)src);
	}
	return;
}
void memset(void* dest, UInt8 data, UInt32 length)
{
	int i;	
	if(dest == 0 || length < 1)
	{
		return;
	}
	for(i = 0; i < length; i++, dest++)
	{
		*((UInt8*)dest) = data;
	}
	return;
}
