#ifndef COMMON_H_
#define COMMON_H_

typedef unsigned int	UInt32;
typedef 	 int	Int32;
typedef unsigned short	UInt16;
typedef 	 short	Int16;
typedef unsigned char	UInt8;
typedef 	 char	Int8;

//I/O
void WriteByte(UInt16 port, UInt8 value);
UInt8 ReadByte(UInt16 port);
UInt16 ReadWord(UInt16 port);

//Memory functions
void memcpy(void* src, void* dest, UInt32 length);
void memset(void* dest, UInt8 data, UInt32 length);
#endif
