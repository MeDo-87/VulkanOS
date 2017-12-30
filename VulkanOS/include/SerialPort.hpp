#ifndef SERIALPORT_HPP_
#define SERIALPORT_HPP_
#include "SerialTypes.hpp"
#include "common.hpp"
#include "isr.hpp"

class SerialPort {
 public:
  // SerialPort();
  SerialPort(Int16 InPort, Int32 InBuadRate, Parity InParity = Parity::NONE,
             CharacterLenght Len = CharacterLenght::EIGHT,
             StopBit InStopBit = StopBit::ONE);
  ~SerialPort();
  void Send(char OutByte);
  void Send(char *OutData, Int32 length);
  char ReadByte();
  void ReadData(char *InData, Int32 length);  // We need better API

  void InterruptHandler(Regs CPURegs);

 private:
  void SetLineControlRegister();
  void DisableInterrupts();
  void SetFIFOControlRegister();
  void SetModemControlRegister();
  void SetBuadRate();
  bool IsReadyToSend();
  bool IsDataAvailable();

  // Store the DelegateObject to call back
 private:
  Int32 BaudRate = 115600;
  UInt16 Port = 0x03F8;
  Parity PortParity = Parity::NONE;
  CharacterLenght CharLength = CharacterLenght::EIGHT;
  StopBit PortStopBit = StopBit::ONE;
  UInt8 TransmissionBuffer[4] = {0, 0, 0, 0};
  UInt8 counter = 0;
  struct PortRegisters {
    UInt8 DataBuffer =
        0;  // Recieve buffer register/Transmit hold register RBR>THR
            // Port +0
    InterruptEnableRegister IER;  // Port +1
    InterruptIDRegister IIR;      // Port +2
    LineControlRegister LCR;      // Port +3
    ModemControlRegister MCR;     // Port +4
    LineStatusRegister LSR;       // Port +5
    ModemStatusRegister MSR;      // Port +6
  } Regs;
  static_assert(sizeof(PortRegisters) == 7, "Wrong port register size");

  static constexpr UInt8 IEROffset = 1;
  static constexpr UInt8 IIROffset = 2;
  static constexpr UInt8 LCROffset = 3;
  static constexpr UInt8 MCROffset = 4;
  static constexpr UInt8 LSROffset = 5;
  static constexpr UInt8 MSROffset = 6;
};

#endif  // SERIALPORT_HPP_