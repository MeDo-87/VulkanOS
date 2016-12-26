#include "common.h"
#include "monitor.h"
#include "descriptorTables.h"
#include "timer.h"
#include "keyboard.h"
int main (struct multiboot *mBoot)
{
	initDescriptorTables();
	asm volatile("sti"); 
	stdio::Monitor::clear();
	stdio::Monitor::SetColour(stdio::Monitor::BLUE,stdio::Monitor::BLACK);
	stdio::Monitor::putString("Ela Rocks\n");
	
	stdio::Monitor::SetColour(stdio::Monitor::WHITE,stdio::Monitor::BLACK);
		
	
	initTimer(18);
	stdio::Keyboard::initKeyboard();
	//putc(35/0);
	return 0xDEADBABE;
}
