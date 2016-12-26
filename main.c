#include "common.h"
#include "monitor.h"
#include "descriptorTables.h"
#include "timer.h"
#include "keyboard.h"
#include "ctordtor.h"

#include "Utils.h"

int main (struct multiboot *mBoot)
{
	(void) mBoot;

	
	initDescriptorTables();
	callConstructors();
	asm volatile("sti"); 
	stdio::Monitor::clear();
	stdio::Monitor::SetColour(stdio::Monitor::BLUE,stdio::Monitor::BLACK);
	stdio::Monitor::putString("Welcome to VulcanOS Ver. 0.0.1\n");
	stdio::Monitor::putString("Developed by MeDo87\n");
	stdio::Monitor::SetColour(stdio::Monitor::WHITE,stdio::Monitor::BLACK);
		
	
	initTimer(18);
	stdio::Keyboard::initKeyboard();
	//putc(35/0);
	callDestructors();
	return 0xDEADBABE;
}
