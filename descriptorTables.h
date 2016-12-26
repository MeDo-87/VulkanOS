#include "common.h"
struct gdtEntry
{
    UInt16 limitLow;   //Lower 16bit of limit
    UInt16 baseLow;    //lower 16bit of base
    UInt8 baseMiddle;  //next 8bits of base
    UInt8 access;      //Access flag, determine ring
    UInt8 granularity; //Granularity
    UInt8 baseHigh;    //last 8bits of base
} __attribute__((packed));

struct gdtPtr
{
    UInt16 limit; //16bit of selector limit
    UInt32 base;  //address of base GDT
} __attribute__((packed));

/*Defines an IDT entry*/
struct idtEntry
{
    UInt16 baseLow;
    UInt16 Select;
    UInt8 Always0;
    UInt8 flags;
    UInt16 baseHigh;
} __attribute__((packed));

/* define IDT pointer */
struct idtPtr
{
    UInt16 limit;
    UInt32 base;
} __attribute__((packed));

// Initialise the descriptors
void initDescriptorTables();



extern "C" {
void gdtFlush(UInt32);
void idtLoad(UInt32);
void isrHandler(struct Regs r);
void initGDT();
void initIDT();
void initIRQ();
void gdtSetGate(UInt32, UInt32, UInt32, UInt8, UInt8);
void idtSetGate(UInt32, UInt32, UInt16, UInt8);
}