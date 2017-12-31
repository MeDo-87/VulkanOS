#ifndef CTOR_H
#define CTOR_H
#include "common.hpp"

extern "C" {
extern void (*__CTOR_LIST__)();
extern void (*__DTOR_LIST__)();
void __cxa_pure_virtual();
}

void callConstructors();
void callDestructors();

void* operator new(UInt32 size);
void operator delete(void* p);

void* operator new[](UInt32 size);
void operator delete[](void* p);

#endif