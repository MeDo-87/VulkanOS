#include "MemoryManager.hpp"
#include "Utils.hpp"
#include "monitor.hpp"

//! 8 blocks per byte
#define PMMNGR_BLOCKS_PER_BYTE 8

//! block size (4k)
#define PMMNGR_BLOCK_SIZE 4096

//! block alignment
#define PMMNGR_BLOCK_ALIGN PMMNGR_BLOCK_SIZE

extern "C" {
void *end(void);
void *start(void);
}

void MemoryManager::init(multiboot_info *mBoot) {

  auto Manager = MemoryManager::Get();

  if (mBoot->flags & 0x01) {
    Manager.TotalPhysicalMemory = 1024 + mBoot->mem_lower + mBoot->mem_upper;
  }
  Manager.MemoryBitMap = (UInt32 *)end;

  //_mmngr_memory_map	=	(uint32_t*) bitmap;
  Manager.MaxMemoryBlocks =
      (Manager.TotalPhysicalMemory * 1024) / PMMNGR_BLOCK_SIZE;

  memory_map_t *mmap = (memory_map_t *)mBoot->mmap_addr;

  // Mark everything as used
  //! By default, all of memory is in use
  memset(Manager.MemoryBitMap, 0xff,
         Manager.MaxMemoryBlocks / PMMNGR_BLOCKS_PER_BYTE);

  GConsole.WriteHex((UInt32)Manager.MemoryBitMap);
  GConsole << "\nMax blocks " << Manager.MaxMemoryBlocks << "\n";

  if (mBoot->flags & 0x20) {
    int i = 0;
    while (mmap < (memory_map_t *)(mBoot->mmap_addr + mBoot->mmap_length)) {

      if (mmap->type == 1) {
        // Mark the memory as available.
        GConsole.SetColour(stdio::Monitor::WHITE, stdio::Monitor::CYAN);
        Manager.InitMemory(mmap->base_addr_low, mmap->length_low);
      } else {
        Manager.ReserveMemory(mmap->base_addr_low, mmap->length_low);
        GConsole.SetColour(stdio::Monitor::WHITE, stdio::Monitor::BLACK);
      }
      GConsole << "Region [" << i << "] starts at: 0x";
      GConsole.WriteHex(mmap->base_addr_high);
      GConsole.WriteHex(mmap->base_addr_low);
      GConsole << " Size 0x";
      GConsole.WriteHex(mmap->length_high);
      GConsole.WriteHex(mmap->length_low);
      GConsole << " With Type ";
      GConsole.WriteDec(mmap->type);
      GConsole << "\n";
      mmap = (memory_map_t *)((unsigned int)mmap + mmap->size +
                              sizeof(mmap->size));
    }
  }
  GConsole.WriteHex((UInt32)end);
  GConsole << "\n";
  GConsole.WriteHex(
      ((UInt32)end + Manager.MaxMemoryBlocks / PMMNGR_BLOCKS_PER_BYTE) /
      PMMNGR_BLOCK_SIZE);
  // Mark the block up to the kernal end + map to be used
  Manager.ReserveMemory(
      0x00, ((UInt32)end + Manager.MaxMemoryBlocks / PMMNGR_BLOCKS_PER_BYTE) /
                PMMNGR_BLOCK_SIZE);

  // Create the Map here
  GConsole << "MemoryManager initialized with ";
  GConsole.WriteDec(Manager.TotalPhysicalMemory);
  GConsole << "\n";
}

// Mark a bit block to be used/Unused.

void MemoryManager::MarkUsed(int page) {
  MemoryBitMap[page / 32] |= (1 << (page % 32));
}

void MemoryManager::MarkUnused(int page) {
  MemoryBitMap[page / 32] &= ~(1 << (page % 32));
}

bool MemoryManager::Test(int page) {
  return MemoryBitMap[page / 32] & (1 << (page % 32));
}

void MemoryManager::InitMemory(UInt32 BaseAdress, UInt32 size) {
  // Calculate the number of blocks we need for this region, and the alignment
  // in our map
  int NumOfBlocks = size / PMMNGR_BLOCK_SIZE;
  int Allign = BaseAdress / PMMNGR_BLOCK_ALIGN;

  // Set the bits corresponding to all the blocks to 0
  while (NumOfBlocks >= 0) {
    MemoryBitMap[Allign] = 0;
    NumOfBlocks--;
    Allign++;
  }
}

void MemoryManager::ReserveMemory(UInt32 BaseAdress, UInt32 size) {
  // Calculate the number of blocks we need for this region, and the alignment
  // in our map
  int NumOfBlocks = size / PMMNGR_BLOCK_SIZE;
  int Allign = BaseAdress / PMMNGR_BLOCK_ALIGN;

  // Set the bits corresponding to all the blocks to 0
  while (NumOfBlocks >= 0) {
    MemoryBitMap[Allign] = 0xFFFFFFFF;
    NumOfBlocks--;
    Allign++;
  }
}

MemoryManager MemoryManager::Get() {
  static MemoryManager Instance = MemoryManager();
  return Instance;
}

MemoryManager::MemoryManager() {}
MemoryManager::~MemoryManager() {}