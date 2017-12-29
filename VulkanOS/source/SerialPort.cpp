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

void SerialPort::SendByte(char OutByte) {
  auto IsReady = [](UInt16 Port) { return ::ReadByte(Port + 5) & 0x20; };
  while (!IsReady(Port)) {
  };

  WriteByte(Port, OutByte);
}
void SerialPort::SendData(char *OutData) {}
char SerialPort::ReadByte() { return '\0'; }
char SerialPort::ReadData() { return '\0'; } // We need better API
