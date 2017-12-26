#include "common.h"
#include "monitor.h"
#include "descriptorTables.h"

using namespace DescriptorTables;

struct GlobalDescriptorTable::Entry gdtEntries[5];
struct GlobalDescriptorTable::Ptr gdtPtr;
struct InterruptDescriptorTable::Entry idtEntries[256];
struct InterruptDescriptorTable::Ptr idtPtr;

void initDescriptorTables()
{
    GlobalDescriptorTable::InitGDT();
    InterruptDescriptorTable::InitIDT();
    InterruptRequest::initIRQ();
}

void GlobalDescriptorTable::InitGDT()
{
    gdtPtr.limit = (sizeof(struct GlobalDescriptorTable::Entry) * 5) - 1;
    gdtPtr.base = (UInt32)&gdtEntries;

    SetGate(0, 0, 0, 0, 0);
    SetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Access = 0b10011010 //Kernel code segment dectriptor
    SetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Access = 0b10010010 //Kernel Data segment
    SetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // Access = 0b11111010 //USER code segment
    SetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // Access = 0b11110010 //User data segment

    gdtFlush((UInt32)&gdtPtr);
}

void GlobalDescriptorTable::SetGate(UInt32 num, UInt32 base, UInt32 limit, UInt8 access, UInt8 gran)
{
    gdtEntries[num].baseLow = base & 0xFFFF;
    gdtEntries[num].baseMiddle = (base >> 16) & 0xFF;
    gdtEntries[num].baseHigh = (base >> 24) & 0xFF;

    gdtEntries[num].limitLow = limit & 0xFFFF;
    gdtEntries[num].granularity = (limit >> 16) & 0x0F;

    gdtEntries[num].granularity |= gran & 0xF0;
    gdtEntries[num].access = access;
}

/* Use this function to set an entry in the IDT. Alot simpler
*  than twiddling with the GDT ;) */
void InterruptDescriptorTable::SetGate(UInt32 num, UInt32 base, UInt16 sel, UInt8 flags)
{
    /* We'll leave you to try and code this function: take the
    *  argument 'base' and split it up into a high and low 16-bits,
    *  storing them in idt[num].base_hi and base_lo. The rest of the
    *  fields that you must set in idt[num] are fairly self-
    *  explanatory when it comes to setup */
    idtEntries[num].baseLow = base & 0xFFFF;
    idtEntries[num].baseHigh = (base >> 16) & 0xFFFF;
    idtEntries[num].Select = sel;
    idtEntries[num].flags = flags;
    idtEntries[num].Always0 = 0;
}

void InterruptDescriptorTable::IRQRemap(void)
{
    //Remap and initialise the PIC
    WriteByte(0x20, 0x11);
    WriteByte(0xA0, 0x11);
    WriteByte(0x21, 0x20);
    WriteByte(0xA1, 0x28);
    WriteByte(0x21, 0x04);
    WriteByte(0xA1, 0x02);
    WriteByte(0x21, 0x01);
    WriteByte(0xA1, 0x01);
    WriteByte(0x21, 0x00);
    WriteByte(0xA1, 0x00);
}

/* Installs the IDT */
void InterruptDescriptorTable::InitIDT()
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    constexpr UInt16 limit = (sizeof(struct InterruptDescriptorTable::Entry) * 256) - 1;
    idtPtr.limit = limit;
    idtPtr.base = (UInt32)&idtEntries;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idtEntries, 0, sizeof(struct InterruptDescriptorTable::Entry) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate */
    SetGate(0, (UInt32)isr0, 0x08, 0x8E);
    SetGate(1, (UInt32)isr1, 0x08, 0x8E);
    SetGate(2, (UInt32)isr2, 0x08, 0x8E);
    SetGate(3, (UInt32)isr3, 0x08, 0x8E);
    SetGate(4, (UInt32)isr4, 0x08, 0x8E);
    SetGate(5, (UInt32)isr5, 0x08, 0x8E);
    SetGate(6, (UInt32)isr6, 0x08, 0x8E);
    SetGate(7, (UInt32)isr7, 0x08, 0x8E);
    SetGate(8, (UInt32)isr8, 0x08, 0x8E);
    SetGate(9, (UInt32)isr9, 0x08, 0x8E);
    SetGate(10, (UInt32)isr10, 0x08, 0x8E);
    SetGate(11, (UInt32)isr11, 0x08, 0x8E);
    SetGate(12, (UInt32)isr12, 0x08, 0x8E);
    SetGate(13, (UInt32)isr13, 0x08, 0x8E);
    SetGate(14, (UInt32)isr14, 0x08, 0x8E);
    SetGate(15, (UInt32)isr15, 0x08, 0x8E);
    SetGate(16, (UInt32)isr16, 0x08, 0x8E);
    SetGate(17, (UInt32)isr17, 0x08, 0x8E);
    SetGate(18, (UInt32)isr18, 0x08, 0x8E);
    SetGate(19, (UInt32)isr19, 0x08, 0x8E);
    SetGate(20, (UInt32)isr20, 0x08, 0x8E);
    SetGate(21, (UInt32)isr21, 0x08, 0x8E);
    SetGate(22, (UInt32)isr22, 0x08, 0x8E);
    SetGate(23, (UInt32)isr23, 0x08, 0x8E);
    SetGate(24, (UInt32)isr24, 0x08, 0x8E);
    SetGate(25, (UInt32)isr25, 0x08, 0x8E);
    SetGate(26, (UInt32)isr26, 0x08, 0x8E);
    SetGate(27, (UInt32)isr27, 0x08, 0x8E);
    SetGate(28, (UInt32)isr28, 0x08, 0x8E);
    SetGate(29, (UInt32)isr29, 0x08, 0x8E);
    SetGate(30, (UInt32)isr30, 0x08, 0x8E);
    SetGate(31, (UInt32)isr31, 0x08, 0x8E);

    IRQRemap();

    SetGate(32, (UInt32)irq0, 0x08, 0x8E);
    SetGate(33, (UInt32)irq1, 0x08, 0x8E);
    SetGate(34, (UInt32)irq2, 0x08, 0x8E);
    SetGate(35, (UInt32)irq3, 0x08, 0x8E);
    SetGate(36, (UInt32)irq4, 0x08, 0x8E);
    SetGate(37, (UInt32)irq5, 0x08, 0x8E);
    SetGate(38, (UInt32)irq6, 0x08, 0x8E);
    SetGate(39, (UInt32)irq7, 0x08, 0x8E);
    SetGate(40, (UInt32)irq8, 0x08, 0x8E);
    SetGate(41, (UInt32)irq9, 0x08, 0x8E);
    SetGate(42, (UInt32)irq10, 0x08, 0x8E);
    SetGate(43, (UInt32)irq11, 0x08, 0x8E);
    SetGate(44, (UInt32)irq12, 0x08, 0x8E);
    SetGate(45, (UInt32)irq13, 0x08, 0x8E);
    SetGate(46, (UInt32)irq14, 0x08, 0x8E);
    SetGate(47, (UInt32)irq15, 0x08, 0x8E);

    /* Points the processor's internal register to the new IDT */
    idtLoad((UInt32)&idtPtr);
}

// All our exception handling interrupt
void isrHandler(struct Regs r)
{
    //ToDo: Interrupt handling
    /*From 0 to 32 */
    //if(r->int_no < 32)
    {
        stdio::Monitor::putString("Interrupt wasraised\nInterrupt number: ");
        stdio::Monitor::WriteDec(r.int_no);
        stdio::Monitor::putString("\nInterrupt Error Code: ");
        stdio::Monitor::WriteDec(r.err_code);
        stdio::Monitor::putc('\n');
    }
}

void InterruptRequest::initIRQ()
{
    return;
}
