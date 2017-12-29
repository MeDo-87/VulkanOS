#ifndef SERIALPORT_HPP_
#define SERIALPORT_HPP_
#include "common.hpp"

enum class Parity : UInt8 { NONE = 0, ODD = 1, EVEN = 3, MARK = 5, SPACE = 7 };

enum class CharacterLenght : UInt8 { FIVE = 0, SIX = 1, SEVEN = 2, EIGHT = 3 };
enum class StopBit : UInt8 { ONE = 0, TWO = 1 };

// Experiment with CRTP

template <typename T> struct IntConverter {
  IntConverter() {
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

struct InterruptEnableRegister // R+W
    : public IntConverter<InterruptEnableRegister> /*+1*/ {
  bool DataAvailable : 1;     // Data Ready interrupt. Raised when data is ready
  bool TransmissionEmpty : 1; // Transmission Empty interrupt. Raised when data
                              // is sent
  bool LineStatus : 1;        // Status Interrupt. Raised due to tx errors
  bool StatusSignal : 1; // Modem Status Interrupt. Raised when delta bits on
                         // MSR is set

private:
  char UNUSED : 4; // Should be zero
};
enum class Interrupt : UInt8 {
  delta_MSR = 0b000,
  ReadyToSend = 0b001,
  DataAvaliable = 0b010,
  Error = 0b011,
  NoFIFO = 0b110

};
/*+2*/ // {
struct InterruptIDRegister : public IntConverter<InterruptIDRegister> {
  struct IID {        /*Read Only Register*/
    bool Pending : 1; // 1 => No Interrupt. 0 = > Interrupt
    Interrupt ID : 3; // 011 => highest. Error. read LST
    // 010 => second Data is available. Service by reading Port offset 0
    // 110 => second No FIFO action. read port offset 0
    // 001 => 3rd, ready to send. Write to port offset 0
    // 000 => lowest, delta flag on MSR is set. Read MSR
  private:
    char UNUSED : 2;

  public:
    bool FIFO : 2; // set if FIFO enabled. clear otherwise.
    // when reading IIR mask bits 4,5,6,7
  };
  struct FCR /*Write Only Register*/ {
    bool enable : 1;
    bool RxReset : 1;
    bool TxReset : 1;

  private:
    bool DMASelect : 1; // clear. not available
    char UNUSED : 2;

  public:
    enum class FIFOBuffer : UInt8 {
      // Size in bytes of the receiver FIFOBuffer
      ONE = 0,
      FOUR = 1,
      EIGHT = 2,
      FOURTEEN = 3
    };
    FIFOBuffer RxBuffer : 2;
  };
  union {
    IID InterruptID;
    FCR FIFOControl;
  };
};

struct LineControlRegister
    : /*r/w*/ public IntConverter<LineControlRegister> /*+3*/ {
  CharacterLenght len : 2;
  StopBit sb : 1;
  Parity parity : 3;

private:
  char UNUSED : 1;

public:
  bool DLAB : 1;
};

struct ModemControlRegister
    : /*r/w*/ public IntConverter<ModemControlRegister> /*+4*/ {
  bool DataTerminalReady : 1;
  bool RequestToSend : 1; //
  UInt8 OUT : 2; // Not used. high bit is connected to interrupt line. Best to
                 // set to 1
  bool LoopBack : 1;

private:
  char UNUSED : 3;
};

// Error detection depending on iterrupt mode that is raised. As well as polled
// mode operation. Same as naive implementation
struct LineStatusRegister : /*r/w*/ public IntConverter<LineStatusRegister> //+5
{
  bool DataAvaliable : 1;
  bool OverrunError : 1;
  bool ParityError : 1;
  bool FrameError : 1;
  bool BrokenLine : 1;
  bool TransmissionEmpty : 1; // Can write new data to send
  bool FinishedSending : 1;   // no transmission is running
  bool FIFOErr : 1;
};

struct ModemStatusRegister
    : /*r/w*/ public IntConverter<ModemStatusRegister> //+6
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
  SerialPort(Int16 InPort, Int32 InBuadRate, Parity InParity = Parity::NONE,
             CharacterLenght Len = CharacterLenght::EIGHT,
             StopBit InStopBit = StopBit::ONE);
  void Send(char OutByte);
  void Send(char *OutData, Int32 length);
  char ReadByte();
  void ReadData(char *InData, Int32 length); // We need better API

private:
  void SetLineControlRegister();
  void DisableInterrupts();
  void SetFIFOControlRegister();
  void SetModemControlRegister();
  void SetBuadRate();
  bool IsReadyToSend();
  bool IsDataAvailable();

private:
  Int32 BaudRate = 115600;
  UInt16 Port = 0x03F8;
  Parity PortParity = Parity::NONE;
  CharacterLenght CharLength = CharacterLenght::EIGHT;
  StopBit PortStopBit = StopBit::ONE;

  struct PortRegisters {
    UInt8 DataBuffer =
        0; // Recieve buffer register/Transmit hold register RBR>THR
           // Port +0
    InterruptEnableRegister IER; // Port +1
    InterruptIDRegister IIR;     // Port +2
    LineControlRegister LCR;     // Port +3
    ModemControlRegister MCR;    // Port +4
    LineStatusRegister LSR;      // Port +5
    ModemStatusRegister MSR;     // Port +6
  } Regs;
  static_assert(sizeof(PortRegisters) == 7, "Wrong port register size");

  static constexpr UInt8 IEROffset = 1;
  static constexpr UInt8 IIROffset = 2;
  static constexpr UInt8 LCROffset = 3;
  static constexpr UInt8 MCROffset = 4;
  static constexpr UInt8 LSROffset = 5;
  static constexpr UInt8 MSROffset = 6;
};

#endif // SERIALPORT_HPP_