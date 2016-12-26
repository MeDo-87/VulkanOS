#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "isr.h"
#include "common.h"
#include "monitor.h"

static UInt8 KeyboardStatus = 0; //00scNCSclA 
namespace stdio
{
 
class Keyboard
{
    public:
    Keyboard();
    static  void initKeyboard();
};

}
#endif
