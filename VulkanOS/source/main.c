#include "MemoryManager.hpp"
#include "SerialPort.hpp"
#include "Utils.hpp"
#include "bootinfo.hpp"
#include "common.hpp"
#include "ctordtor.hpp"
#include "descriptorTables.hpp"
#include "keyboard.hpp"
#include "monitor.hpp"
#include "timer.hpp"

int main(struct multiboot_info *mBoot) {
  
  UInt32 memSize = 1024 + mBoot->mem_lower + mBoot->mem_upper;

  initDescriptorTables();
  callConstructors();
  asm volatile("sti");
  GConsole.clear();
  GConsole.SetColour(stdio::Monitor::BLUE, stdio::Monitor::BLACK);
  GConsole << "Welcome to VulcanOS Ver. 0.0.1\n";
  GConsole << "Developed by MeDo87\n";
  GConsole.SetColour(stdio::Monitor::WHITE, stdio::Monitor::BLACK);
  
  MemoryManager::init(mBoot);

  //! initialize the physical memory manager
  //! we place the memory bit map used by the PMM at the end of the kernel in
  //! memory

  GConsole << "pmm initialized with ";
  GConsole.WriteDec(memSize);
  GConsole << " KB physical memory;";
  GConsole.WriteDec(mBoot->mem_lower);
  GConsole << " KB Low memory;";
  GConsole.WriteDec(mBoot->mem_upper);
  GConsole << " blocks of 64KB High memory;\n";

  Timer::initTimer(100);
  stdio::Keyboard::initKeyboard();

  DebugBreak();
  {
    auto Port = SerialPort(0x03F8, 9600);
    GConsole << "Writing to serial 1\n";
    Port.Send('A');
    Port.Send('B');
    Port.Send('C');
  }
  DebugBreak();
  
  callDestructors();
  return 0xDEADBABE;
}
