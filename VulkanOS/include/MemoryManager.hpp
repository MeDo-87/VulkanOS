//Memory management class
//Constructed immedietly when the kernel is loaded using the MultiBoot Boot Info structure data.
#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H
#include "bootinfo.hpp"
#include "common.hpp"

class MemoryManager
{
public:
   static void init (multiboot_info* _multiBoot);
   MemoryManager();
   void MarkUsed(int page);
   void MarkUnused(int page);
   bool Test(int page);
   static MemoryManager Get();
    ~MemoryManager();
protected:
    UInt32 TotalPhysicalMemory = 0;
    UInt32 UsedMemoryBlocks = 0;
    UInt32 MaxMemoryBlocks = 0;
    UInt32* MemoryBitMap = nullptr;
private:
    void InitMemory(UInt32 baseAddress, UInt32 size);
    void ReserveMemory(UInt32 BaseAdress, UInt32 size);

};

#endif