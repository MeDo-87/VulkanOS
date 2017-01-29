#ifndef TIMER_H
#define TIMER_H

#include "isr.h"
#include "common.h"
#include "monitor.h"

class Timer
{
public:
    static void initTimer(UInt32 frequency);

private:
    static UInt32 Tick;
    static void TimerTick(struct Regs r);
    
};
#endif

