#include "isr.hpp"
#include "common.hpp"
#include "ctordtor.hpp"
#include "descriptorTables.hpp"
#include "monitor.hpp"
DelegateBase<void, Regs> *irqRoutines[16] = {0, 0, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0};

void InstallIrqHandler(UInt32 irq, DelegateBase<void, Regs> *Delegate) {
  irqRoutines[irq] = Delegate;
}

void UninstallIrqHandler(UInt32 irq) {
  delete irqRoutines[irq];
  irqRoutines[irq] = 0;
}

// Need to acknowledge the IRQ and raise end of IRQ to PIC
void irqHandler(struct Regs r) {
  if (irqRoutines[r.int_no - 32] != 0) {
    auto handler = irqRoutines[r.int_no - 32];
    (*handler)(r);
  }
  // Send the end of Intrerupt command
  if (r.int_no >= 40) {
    WriteByte(0xA0, 0x20);
  }
  WriteByte(0x20, 0x20);
}
