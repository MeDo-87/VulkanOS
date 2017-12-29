#ifndef SERIALPORT_HPP_
#define SERIALPORT_HPP_
#include "common.hpp"

enum class Parity : UInt8 { NONE = 0, ODD = 1, EVEN = 3, MARK = 5, SPACE = 7 };

enum class CharacterLenght : UInt8 { FIVE = 0, SIX = 1, SEVEN = 2, EIGHT = 3 };
enum class StopBit : UInt8 { ONE = 0, TWO = 1 };

// Experiment with CRTP

template <typename T> struct IntConverter {
  operator UInt8() const {
    const T *derived = static_cast<const T *>(this);
    auto IntPtr = reinterpret_cast<const UInt8 *>(derived);
    return *(IntPtr);
  };
};

struct InterruptEnableRegister
    : public IntConverter<InterruptEnableRegister> /*+1*/ {
  bool DataAvailable : 1;
  bool TransmissionEmpty : 1;
  bool LineStatus : 1;
  bool StatusSignal : 1;

private:
  char UNUSED : 4;
};

// struct  : public IntConverter<InterruptIDRegister> /*+2*/ {
union InterruptIDRegister {
  struct IID : public IntConverter<IID> {
    bool Pending : 1;
    bool ID : 3;

  private:
    char UNUSED : 2;

  public:
    bool FIFO : 2;
  };
  struct FCR : public IntConverter<FCR> {
    bool enable : 1;
    bool RxReset : 1;
    bool TxReset : 1;
    bool DMASelect : 1;

  private:
    char UNUSED : 2;

  public:
    bool RxTrigger : 2;
  };
};

struct LineControlRegister : public IntConverter<LineControlRegister> /*+3*/ {
  CharacterLenght len : 2;
  StopBit sb : 1;
  Parity parity : 3;

private:
  char UNUSED : 1;

public:
  bool DLAB : 1;

public:
  LineControlRegister()
      : len(CharacterLenght::FIVE), sb(StopBit::ONE), parity(Parity::NONE),
        UNUSED(0), DLAB(false){};
};

struct ModemControlRegister : public IntConverter<ModemControlRegister> /*+4*/ {
  bool DataTerminalReady : 1;
  bool RequestToSend : 1; //
  bool OUT : 2;           // Not used. high bit is connected to interrupt line
  bool LOOP : 1;

private:
  char UNUSED : 3;
};

struct LineStatusRegister : public IntConverter<LineStatusRegister> //+5
{
  bool DataAvaliable : 1;
  bool OverrunError : 1;
  bool ParityError : 1;
  bool FrameError : 1;
  bool BrokenLine : 1;
  bool TransmissionEmpty : 1;
  bool FinishedSending : 1;
  bool FIFOErr : 1;
};

struct ModemStatusRegister : public IntConverter<ModemStatusRegister> //+6
{
  bool DeltaClearToSend : 1;
  bool DeltaReadyToSend : 1;
  bool TrailingEdge : 1;
  bool DeltaDataCarrier : 1;
  bool ClearToSend : 1;
  bool DataSetReady : 1;
  bool RingIndicator : 1;
  bool DataCarrierDetect : 1;
};

class SerialPort {
public:
  SerialPort();
  SerialPort(Int8 InPort, Int32 InBuadRate, Parity InParity = Parity::NONE,
             CharacterLenght Len = CharacterLenght::EIGHT,
             StopBit InStopBit = StopBit::ONE);
  void SendByte(char OutByte);
  void SendData(char *OutData);
  char ReadByte();
  char ReadData(); // We need better API

private:
  Int32 BaudRate = 115600;
  Int8 Port = 0x03F8;
  Parity PortParity = Parity::NONE;
  CharacterLenght CharLength = CharacterLenght::EIGHT;
  StopBit PortStopBit = StopBit::ONE;

  InterruptEnableRegister IntReg;

  LineControlRegister LCReg;
};

#endif // SERIALPORT_HPP_