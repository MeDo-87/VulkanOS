#include "SerialPort.hpp"
#include "Utils.hpp"
constexpr UInt32 PORTCLOCK = 115200;

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

SerialPort::SerialPort() {}
SerialPort::SerialPort(Int16 InPort, Int32 InBaudRate, Parity InParity,
                       CharacterLenght Len, StopBit InStopBit)
    : Port(InPort), BaudRate(InBaudRate), PortParity(InParity), CharLength(Len),
      PortStopBit(InStopBit)

{

  DisableInterrupts();
  Regs.LCR.DLAB = true;
  WriteByte(Port + LCROffset, static_cast<UInt8>(Regs.LCR));
  SetBuadRate();

  SetLineControlRegister();
  SetFIFOControlRegister();
  SetModemControlRegister();
}

void SerialPort::SetBuadRate() {
  UInt16 buaddivider = PORTCLOCK / BaudRate;
  UInt8 *bd = reinterpret_cast<UInt8 *>(&buaddivider);

  WriteByte(Port, bd[0]); // TODO: Doesn't work
  WriteByte(Port + 1, bd[1]);
}

void SerialPort::SetModemControlRegister() {
  Regs.MCR.DataTerminalReady = 1;
  Regs.MCR.RequestToSend = 1;
  Regs.MCR.OUT = 2;
  WriteByte(Port + MCROffset, Regs.MCR);
}
void SerialPort::SetFIFOControlRegister() {
  Regs.IIR.FIFOControl.enable = true;
  Regs.IIR.FIFOControl.RxReset = true;
  Regs.IIR.FIFOControl.TxReset = true;
  Regs.IIR.FIFOControl.RxBuffer =
      InterruptIDRegister::FCR::FIFOBuffer::FOURTEEN;
  WriteByte(Port + IIROffset, static_cast<UInt8>(Regs.IIR));
}
void SerialPort::DisableInterrupts() {
  Regs.IER.DataAvailable = false;
  Regs.IER.TransmissionEmpty = false;
  Regs.IER.StatusSignal = false;
  Regs.IER.LineStatus = false;
  WriteByte(Port + IEROffset, Regs.IER);
}

void SerialPort::SetLineControlRegister() {
  Regs.LCR.len = CharLength;
  Regs.LCR.parity = PortParity;
  Regs.LCR.sb = PortStopBit;
  Regs.LCR.DLAB = false;
  WriteByte(Port + LCROffset, Regs.LCR);
}

bool SerialPort::IsReady() {

  Regs.LSR.FromInt8(::ReadByte(Port + LSROffset));
  return Regs.LSR.TransmissionEmpty;
}

void SerialPort::Send(char OutByte) {

  while (!IsReady()) {
  };
  WriteByte(Port, OutByte);
}
void SerialPort::Send(char *OutData, Int32 length) {
  while (!IsReady()) {
  };
  for (Int32 i = 0; i < length; i++) {
    WriteByte(Port, OutData[i]);
  }
}
char SerialPort::ReadByte() { return '\0'; }
Int32 SerialPort::ReadData(char *InData) { return '\0'; } // We need better API
