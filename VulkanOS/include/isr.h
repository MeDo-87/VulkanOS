#ifndef ISR_H
#define ISR_H
#include "common.h"
#ifdef __cplusplus /* only defined in C++ code */
extern "C" {
#endif

/* This defines what the stack looks like after an ISR was running */
struct Regs
{
    UInt32 /*gs, fs, es, */ds;      /* pushed the segs last */
    UInt32 edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    UInt32 int_no, err_code;    /* our 'push byte #' and ecodes do this */
    UInt32 eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

void InstallIrqHandler(UInt32 irq, void (*handler)(struct Regs r));
void UninstallIrqHandler(UInt32 irq);

void irqHandler(struct Regs r);

//Interrupt service routines defined in Athe assembly
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10 ();
extern void isr11 ();
extern void isr12 ();
extern void isr13 ();
extern void isr14 ();
extern void isr15 ();
extern void isr16 ();
extern void isr17 ();
extern void isr18 ();
extern void isr19 ();
extern void isr20 ();
extern void isr21 ();
extern void isr22 ();
extern void isr23 ();
extern void isr24 ();
extern void isr25 ();
extern void isr26 ();
extern void isr27 ();
extern void isr28 ();
extern void isr29 ();
extern void isr30 ();
extern void isr31(); 

//Hardware interrupts service routine
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#ifdef __cplusplus /* only defined in C++ code */
}
#endif

#endif
