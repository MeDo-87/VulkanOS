#include "keyboard.h"
#include "Utils.h"

constexpr int CTRL = 1;
constexpr int SHIFT = 2;
constexpr int ALT = 3;
constexpr int CAPS = 4;
constexpr int NUMLOCK = 5;
constexpr int SCROL = 6;

static UInt8 KeyboardStatus = 0;  // 00scNCSclA
unsigned char qwerty[128] = {
    0,       27,  '1',   '2', '3',   '4', '5', '6', '7',  '8', /* 9 */
    '9',     '0', '-',   '=', '\b',                            /* Backspace */
    '\t',                                                      /* Tab */
    'q',     'w', 'e',   'r',                                  /* 19 */
    't',     'y', 'u',   'i', 'o',   'p', '[', ']', '\n',      /* Enter key */
    CTRL, /* 29   - Control */
    'a',     's', 'd',   'f', 'g',   'h', 'j', 'k', 'l',  ';', /* 39 */
    '\'',    '`', SHIFT,                                       /* Left shift */
    '\\',    'z', 'x',   'c', 'v',   'b', 'n',                 /* 49 */
    'm',     ',', '.',   '/', SHIFT,                           /* Right shift */
    '*',     ALT,                                              /* Alt */
    ' ',                                                       /* Space bar */
    CAPS,                                                      /* Caps lock */
    0,                                                 /* 59 - F1 key ... > */
    0,       0,   0,     0,   0,     0,   0,   0,   0, /* < ... F10 */
    NUMLOCK,                                           /* 69 - Num lock*/
    SCROL,                                             /* Scroll Lock */
    0,                                                 /* Home key */
    0,                                                 /* Up Arrow */
    0,                                                 /* Page Up */
    '-',     0,                                        /* Left Arrow */
    0,       0,                                        /* Right Arrow */
    '+',     0,                                        /* 79 - End key*/
    0,                                                 /* Down Arrow */
    0,                                                 /* Page Down */
    0,                                                 /* Insert Key */
    0,                                                 /* Delete Key */
    0,       0,   0,     0,                            /* F11 Key */
    0,                                                 /* F12 Key */
    0, /* All other keys are undefined */
};

static void keyboardCallback(struct Regs r) {
  (void)r;
  UInt8 scancode;
  UInt8 ascii = 0;
  /* Read from the keyboard's data buffer */
  scancode = ReadByte(0x60);

  /* If the top bit of the byte we read from the keyboard is
*  set, that means that a key has just been released */
  if (scancode & 0x80) {
    /* You can use this one to see if the user released the
*  shift, alt, or control keys... */
    // ascii &= 0b01111111;
    ascii = qwerty[scancode & 0b01111111];
    if (ascii == CAPS || ascii == NUMLOCK || ascii == SCROL) {
      while ((ReadByte(0x64) & 2) != 0)
        ;
      WriteByte(0x60, 0xED);
      // while ((ReadByte(0x64) & 2) != 0);
      UInt8 lights = KeyboardStatus >> 3;
      WriteByte(0x60, lights);
    } else {
      switch (ascii) {
        case ALT:
          KeyboardStatus ^= 0b00000001;
          break;
        case CTRL:
          KeyboardStatus ^= 0b00000010;
          break;
        case SHIFT:
          KeyboardStatus ^= 0b00000100;
          break;
      }
    }
  } else {
    ascii = qwerty[scancode];
    UInt8 caps = 0;
    UInt8 shift = ((KeyboardStatus >> 2) & 0x01);
    UInt8 capslock = ((KeyboardStatus >> 5) & 0x01);
    switch (ascii) {
      case ALT:
        KeyboardStatus ^= 0b00000001;
        break;
      case CTRL:
        KeyboardStatus ^= 0b00000010;
        break;
      case SHIFT:
        KeyboardStatus ^= 0b00000100;
        break;
      case SCROL:
        KeyboardStatus ^= 0b00001000;
        break;
      case NUMLOCK:
        KeyboardStatus ^= 0b00010000;
        break;
      case CAPS:
        KeyboardStatus ^= 0b00100000;
        break;
      default:
        /* Here, a key was just pressed. Please note that if you
  *  hold a key down, you will get repeated key press
  *  interrupts. */

        /* Just to show you how this works, we simply translate
  *  the keyboard scancode into an ASCII value, and then
  *  display it to the screen. You can get creative and
  *  use some flags to see if a shift is pressed and use a
  *  different layout, or you can add another 128 entries
  *  to the above layout to correspond to 'shift' being
  *  held. If shift is held using the larger lookup table,
  *  you would add 128 to the scancode when you look for it */

        if ((capslock || shift) && ascii < 123 && ascii > 96) {
          caps = 32;
        }
        stdio::Monitor::putc(ascii - caps);
    }
  }
}

using namespace stdio;

Keyboard::Keyboard() {
  // Initialise the keyboard;
  InstallIrqHandler(1, &keyboardCallback);
}
void Keyboard::initKeyboard() {
  InstallIrqHandler(1, &keyboardCallback);
  GConsole << "Keyboard Installed";
}
