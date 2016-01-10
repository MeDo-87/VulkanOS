#include "common.h"
#include "monitor.h"
#include "descriptorTables.h"
#include "timer.h"
#include "keyboard.h"
int main (struct multiboot *mBoot)
{
	initDescriptorTables();
	asm volatile("sti"); 
	clear();
	SetColour(BLUE,BLACK);
	putString("Ela Rocks\n");
	
	SetColour(WHITE,BLACK);
		
	
	initTimer(18);
	initKeyboard();
	//putc(35/0);
	return 0xDEADBABE;
}
