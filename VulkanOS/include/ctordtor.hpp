#ifndef CTOR_H
#define CTOR_H
#include "common.hpp"

extern "C" {
extern void (*__CTOR_LIST__)();
extern void (*__DTOR_LIST__)();
void __cxa_pure_virtual();
void __cxa_atexit();
void __cxa_guard_acquire();
void *__dso_handle();
void __cxa_guard_release();
}

void callConstructors();
void callDestructors();

void *operator new(UInt32 size);
void operator delete(void *p);
void operator delete(void *p, UInt32 size);

void *operator new[](UInt32 size);
void operator delete[](void *p);

#endif