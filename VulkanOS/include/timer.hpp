#ifndef TIMER_H
#define TIMER_H

#include "common.hpp"
#include "isr.hpp"
#include "monitor.hpp"

class Timer {
 public:
  static void initTimer(UInt32 frequency);

 private:
  static UInt32 Tick;
  static void TimerTick(struct Regs r);
};
#endif
