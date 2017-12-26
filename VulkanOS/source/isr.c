#include "common.h"
#include "monitor.h"
#include "descriptorTables.h"
#include "isr.h"



void *irqRoutines[16] = {0,0,0,0,0,0,0,0,
			 0,0,0,0,0,0,0,0};

void InstallIrqHandler(UInt32 irq, void (*handler)(struct Regs r))
{
	irqRoutines[irq] = reinterpret_cast<void*>(handler);
}

void UninstallIrqHandler(UInt32 irq)
{
	irqRoutines[irq] = 0;
}

//Need to acknowledge the IRQ and raise end of IRQ to PIC
void irqHandler(struct Regs r)
{
	if(irqRoutines[r.int_no-32] != 0)
	{
		void (*handler)(struct Regs r);
		handler = reinterpret_cast<void (*)(struct Regs r)>(irqRoutines[r.int_no-32]);
		handler(r);
	}
    // Send the end of Intrerupt command   
	if(r.int_no >= 40)
	{
		WriteByte(0xA0, 0x20);
	}
	WriteByte(0x20, 0x20);
} 

