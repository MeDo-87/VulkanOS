#ifndef SERIAL_TYPES_HPP_
#define SERIAL_TYPES_HPP_
#include "Utilities/TypeConverter.hpp"
#include "common.hpp"

enum class Parity : UInt8 {
  NONE = 0b000,
  ODD = 0b001,
  EVEN = 0b011,
  MARK = 0b101,
  SPACE = 0b111
};

enum class CharacterLenght : UInt8 {
  FIVE = 0b00,
  SIX = 0b01,
  SEVEN = 0b10,
  EIGHT = 0b11
};

enum class StopBit : UInt8 { ONE = 0b0, TWO = 0b1 };

enum class Interrupt : UInt8 {
  delta_MSR = 0b000,      // LOWEST, read MSR
  ReadyToSend = 0b001,    // THIRD, send byte
  DataAvaliable = 0b010,  // SECOND, read port buffer
  Error = 0b011,          // HIGHEST, Read LST to clear
  NoFIFO = 0b110          // SECOND, read port buffer
};

enum class FIFOBuffer : UInt8 {
  // Size in bytes of the receiver FIFOBuffer
  ONE = 0b00,
  FOUR = 0b01,
  EIGHT = 0b10,
  FOURTEEN = 0b11
};

struct InterruptEnableRegister  // R+W
    : public IntConvertable<InterruptEnableRegister> /*+1*/ {
  bool DataAvailable : 1;  // Data Ready interrupt. Raised when data is ready
  bool TransmissionEmpty : 1;  // Transmission Empty interrupt. Raised when data
                               // is sent
  bool LineStatus : 1;         // Status Interrupt. Raised due to tx errors
  bool StatusSignal : 1;  // Modem Status Interrupt. Raised when delta bits on
                          // MSR is set

 private:
  char UNUSED : 4;  // Should be zero
};

/*+2*/  // {
struct InterruptIDRegister : public IntConvertable<InterruptIDRegister> {
  struct IID {         /*Read Only Register*/
    bool Pending : 1;  // 1 => No Interrupt. 0 = > Interrupt
    Interrupt ID : 3;

   private:
    char UNUSED : 2;

   public:
    bool FIFO : 2;  // set if FIFO enabled. clear otherwise.
  };
  struct FCR /*Write Only Register*/ {
    bool Enable : 1;
    bool RxReset : 1;
    bool TxReset : 1;

   private:
    bool DMASelect : 1;  // clear. not available
    char UNUSED : 2;

   public:
    FIFOBuffer RxBuffer : 2;
  };

  union {
    IID InterruptID;
    FCR FIFOControl;
  };
};

struct LineControlRegister
    : /*r/w*/ public IntConvertable<LineControlRegister> /*+3*/ {
  CharacterLenght len : 2;
  StopBit sb : 1;
  Parity parity : 3;

 private:
  char UNUSED : 1;

 public:
  bool DLAB : 1;
};

struct ModemControlRegister
    : /*r/w*/ public IntConvertable<ModemControlRegister> /*+4*/ {
  bool DataTerminalReady : 1;
  bool RequestToSend : 1;  //
  UInt8 OUT : 2;  // Not used. high bit is connected to interrupt line. Best to
                  // set to 1
  bool LoopBack : 1;

 private:
  char UNUSED : 3;
};

// Error detection depending on iterrupt mode that is raised. As well as polled
// mode operation.
struct LineStatusRegister
    : /*r/w*/ public IntConvertable<LineStatusRegister>  //+5
{
  bool DataAvaliable : 1;
  bool OverrunError : 1;
  bool ParityError : 1;
  bool FrameError : 1;
  bool BrokenLine : 1;
  bool TransmissionEmpty : 1;  // Can write new data to send
  bool FinishedSending : 1;    // no transmission is running
  bool FIFOErr : 1;
};

struct ModemStatusRegister
    : /*r/w*/ public IntConvertable<ModemStatusRegister>  //+6
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

static_assert(sizeof(InterruptEnableRegister) == 1,
              "InterruptEnableRegister must be 1 byte long");
static_assert(sizeof(InterruptIDRegister) == 1,
              "InterruptIDRegister must be 1 byte long");
static_assert(sizeof(LineControlRegister) == 1,
              "LineControlRegister must be 1 byte long");
static_assert(sizeof(ModemControlRegister) == 1,
              "ModemControlRegister must be 1 byte long");
static_assert(sizeof(LineStatusRegister) == 1,
              "LineStatusRegister must be 1 byte long");
static_assert(sizeof(ModemStatusRegister) == 1,
              "ModemStatusRegister must be 1 byte long");
#endif