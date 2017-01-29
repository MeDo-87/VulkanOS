#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "isr.h"
#include "common.h"
#include "monitor.h"


namespace stdio
{
 
class Keyboard
{
    public:
    Keyboard();
    ~Keyboard();
    static  void initKeyboard();
};

}
#endif
