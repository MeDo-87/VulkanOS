#ifndef TYPE_CONVERTER_HPP_
#define TYPE_CONVERTER_HPP_
#include "common.hpp"
// Experiment with CRTP
template <typename T>
struct IntConvertable {
  IntConvertable() {
    T *derived = static_cast<T *>(this);
    memset(derived, 0, sizeof(T));
  }
  T FromInt8(UInt8 InByte) {
    T *derived = static_cast<T *>(this);
    memset(derived, InByte, sizeof(UInt8));
    return *derived;
  }
  operator UInt8() const {
    const T *derived = static_cast<const T *>(this);
    auto IntPtr = reinterpret_cast<const UInt8 *>(derived);
    return *(IntPtr);
  };
};
#endif