#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "isr.hpp"
#include "common.hpp"
#include "monitor.hpp"


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
