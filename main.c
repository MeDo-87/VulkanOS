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
	putString("Ela Rocks\n");
	
		WriteDec(10);
		putc(' ');

	WriteDec(2564);
	putc(' ');
		
	
	initTimer(18);
	initKeyboard();
	//putc(35/0);
	return 0xDEADBABE;
}
