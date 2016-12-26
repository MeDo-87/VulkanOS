
#include "common.h"
extern "C" {
  extern void (*__CTOR_LIST__)() ;
  extern void (*__DTOR_LIST__)() ;
}


void callConstructors();
void callDestructors();


void* operator new(UInt32 size);
void operator delete(void* p);

void* operator new[](UInt32 size);
void operator delete[](void* p);