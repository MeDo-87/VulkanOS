#include "SerialPort.hpp"

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
SerialPort::SerialPort(Int8 InPort, Int32 InBaudRate, Parity InParity,
                       CharacterLenght Len, StopBit InStopBit)
    : Port(InPort), BaudRate(InBaudRate), PortParity(InParity), CharLength(Len),
      PortStopBit(InStopBit)

{

  WriteByte(Port + 1, 0); // Disable interrupt
  LCReg.DLAB = true;
  WriteByte(Port + 3, static_cast<UInt8>(LCReg));

  UInt16 buaddivider = PORTCLOCK / BaudRate;
  WriteWord(Port, buaddivider);

  LCReg.len = CharLength;
  LCReg.parity = PortParity;
  LCReg.sb = PortStopBit;
  LCReg.DLAB = false;
  WriteByte(Port + 3, LCReg);

  IntReg.DataAvailable = false;
  IntReg.TransmissionEmpty = false;
  IntReg.StatusSignal = false;
  IntReg.LineStatus = false;
  WriteByte(Port + 1, IntReg);
}

void SerialPort::SendByte(char OutByte) {
  auto IsReady = [](UInt8 Port) { return ::ReadByte(Port + 5) & 0x20; };

  while (IsReady(Port)) {
  };

  char t = ::ReadByte(Port);
}
void SerialPort::SendData(char *OutData) {}
char SerialPort::ReadByte() { return '\0'; }
char SerialPort::ReadData() { return '\0'; } // We need better API
