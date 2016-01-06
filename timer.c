#include "monitor.h"
#include "timer.h"
UInt32 tick = 0;

static void timerCallback(struct Regs r)
{
	tick++;
	//putString("Tick: ");
	//WriteDec(tick);
	//putc('\n');
	
}

void initTimer(UInt32 frequency)
{
	InstallIrqHandler(0,&timerCallback);

	UInt32 divisor = 1193180 / frequency;
	WriteByte(0x43, 0x36); //command that we are setting the divisor
	UInt8 l = (UInt8) (divisor & 0xFF);
	UInt8 h = (UInt8) ((divisor >> 8));	
	
	WriteByte(0x40,l);
	WriteByte(0x40,h);
}
