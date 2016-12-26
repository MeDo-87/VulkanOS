#ifndef COMMON_H_
#define COMMON_H_

typedef long unsigned int UInt32;
typedef int Int32;
typedef unsigned short UInt16;
typedef short Int16;
typedef unsigned char UInt8;
typedef char Int8;

typedef UInt32 size_t;

static_assert(sizeof(UInt8) == 1, "uint8_t must be 1 byte long");
static_assert(sizeof(UInt16) == 2, "uint16_t must be 2 bytes long");
static_assert(sizeof(UInt32) == 4, "uint32_t must be 4 bytes long");

#ifdef __cplusplus /* only defined in C++ code */
extern "C" {
#endif
//I/O
inline void WriteByte(UInt16 port, UInt8 value)
{
    asm volatile("outb %1, %0"
		 :
		 : "dN"(port), "a"(value));
}
inline UInt8 ReadByte(UInt16 port)
{
    UInt8 ret;
    asm volatile("inb %1, %0"
		 : "=a"(ret)
		 : "dN"(port));
    return ret;
}

UInt16 ReadWord(UInt16 port);

//Memory functions
void memcpy(void *src, void *dest, UInt32 length);
void memset(void *dest, UInt8 data, UInt32 length);

#ifdef __cplusplus /* only defined in C++ code */
}
#endif

#endif
