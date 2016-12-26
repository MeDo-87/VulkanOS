#include "common.h"
#include "isr.h"
namespace DescriptorTables
{

class GlobalDescriptorTable
{
  public:
    struct Entry
    {
        UInt16 limitLow;   //Lower 16bit of limit
        UInt16 baseLow;    //lower 16bit of base
        UInt8 baseMiddle;  //next 8bits of base
        UInt8 access;      //Access flag, determine ring
        UInt8 granularity; //Granularity
        UInt8 baseHigh;    //last 8bits of base
    } __attribute__((packed));

    struct Ptr
    {
        UInt16 limit; //16bit of selector limit
        UInt32 base;  //address of base GDT
    } __attribute__((packed));

    static void InitGDT();
    static void SetGate(UInt32 num, UInt32 base, UInt32 limit, UInt8 access, UInt8 gran);

  private:
    GlobalDescriptorTable() = delete;
    GlobalDescriptorTable(const GlobalDescriptorTable &other) = delete;
    ~GlobalDescriptorTable() = delete;
    GlobalDescriptorTable &operator=(const GlobalDescriptorTable &) = delete;
} __attribute__((packed));

class InterruptDescriptorTable
{
  public:
    /*Defines an IDT entry*/
    struct Entry
    {
        UInt16 baseLow;
        UInt16 Select;
        UInt8 Always0;
        UInt8 flags;
        UInt16 baseHigh;
    } __attribute__((packed));

    /* define IDT pointer */
    struct Ptr
    {
        UInt16 limit;
        UInt32 base;
    } __attribute__((packed));

    static void InitIDT();
    static void SetGate(UInt32, UInt32, UInt16, UInt8);
    static void IRQRemap(void);

  private:
    InterruptDescriptorTable() = delete;
    InterruptDescriptorTable(const InterruptDescriptorTable &other) = delete;
    ~InterruptDescriptorTable() = delete;
    InterruptDescriptorTable &operator=(const InterruptDescriptorTable &) = delete;
};

class InterruptRequest
{
  public:
    static void initIRQ();
};
}

// Initialise the descriptors
void initDescriptorTables();

extern "C" {
void gdtFlush(UInt32);
void idtLoad(UInt32);
void isrHandler(struct Regs r);
}