#include "ctordtor.hpp"
#include "Utils.hpp"
#include "heap.hpp"
void callConstructors() {
  void (**constructor)() = &__CTOR_LIST__;
  int total = *(int *)constructor;
  constructor++;
  while (total) {
    (*constructor)();
    total--;
    constructor++;
  }
}
void callDestructors() {
  void (**deconstructor)() = &__DTOR_LIST__;
  int total = *(int *)deconstructor;
  deconstructor++;
  while (total) {
    (*deconstructor)();
    total--;
    deconstructor++;
  }
}

void *operator new(UInt32 size) { return Allocator::kmalloc(size); }
void operator delete(void *p) {}
void operator delete(void *p, UInt32 size) {}

void *operator new[](UInt32 size) { return Allocator::kmalloc(size); }
void operator delete[](void *p) {}
void operator delete[](void *p, UInt32 size) {}

void __cxa_pure_virtual() {}
