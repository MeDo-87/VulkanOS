#ifndef DELEGATE_HPP__
#define DELEGATE_HPP__

template <typename ReturnType, typename ArgType> class DelegateBase {
public:
  virtual ReturnType operator()(ArgType) = 0;
};

template <typename T, typename ReturnType, typename ArgType>
class Delegate : public DelegateBase<ReturnType, ArgType> {
  typedef ReturnType (T::*FuncType)(ArgType);

private:
  T *Object = nullptr;
  FuncType Func = nullptr;

public:
  Delegate(T *InObject, FuncType InFunc) : Object(InObject), Func(InFunc){};
  ReturnType operator()(ArgType Arg1) override {
    if (Object && Func) {
      return (Object->*Func)(Arg1);
    }
  }
};

template <class ReturnType, typename ArgType>
class Delegate<void, ReturnType, ArgType>
    : public DelegateBase<ReturnType, ArgType> {
  typedef ReturnType (*FuncType)(ArgType);

private:
  FuncType Func;

public:
  Delegate(FuncType InFunc) : Func(InFunc){};
  ReturnType operator()(ArgType Arg1) { Func(Arg1); }
};

template <typename T> using VoidDelegate = Delegate<T, void, void>;
template <typename T, typename ArgType>
using DelegateOneArg = Delegate<T, void, ArgType>;
template <typename ReturnType, typename ArgType>
using FreeDelegate = Delegate<void, ReturnType, ArgType>;
#endif