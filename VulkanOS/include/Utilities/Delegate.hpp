#ifndef DELEGATE_HPP__
#define DELEGATE_HPP__

template <typename ReturnType> class DelegateBase {
public:
  ReturnType operator() = 0;
};

template <typename T, typename ReturnType>
class Delegate : public DelegateBase<ReturnType> {
  typedef void (T::*FuncType)();

private:
  T *Object;
  FuncType Func;

public:
  Delegate(T *InObject, FuncType InFunc) : Object(InObject), Func(InFunc){};
  ReturnType operator() { Object->*Func(); }
};

#endif