#ifndef CTOR_H
#define CTOR_H
#include "common.h"

extern "C" {
  extern void (*__CTOR_LIST__)() ;
  extern void (*__DTOR_LIST__)() ;
  void __cxa_pure_virtual ();
  void  __cxa_atexit();
}


void callConstructors();
void callDestructors();


void* operator new(UInt32 size);
void operator delete(void* p);

void* operator new[](UInt32 size);
void operator delete[](void* p);

void * __dso_handle();

#endif