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
	GConsole.clear();
	GConsole.SetColour(stdio::Monitor::BLUE,stdio::Monitor::BLACK);
	GConsole << "Welcome to VulcanOS Ver. 0.0.1\n";
	GConsole << "Developed by MeDo87\n";
	GConsole.SetColour(stdio::Monitor::WHITE,stdio::Monitor::BLACK);
		
	
	initTimer(18);
	stdio::Keyboard::initKeyboard();
	//putc(35/0);
	callDestructors();
	return 0xDEADBABE;
}
