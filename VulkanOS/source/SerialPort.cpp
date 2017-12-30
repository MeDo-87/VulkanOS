#include "SerialPort.hpp"
#include "Utils.hpp"

constexpr UInt32 PORTCLOCK = 115200;

// SerialPort::SerialPort() {}
void SerialPort::InterruptHandler(struct Regs CPURegs) {
  (void)CPURegs;
  Regs.IIR.FromInt8(::ReadByte(0x03F8 + 2));

  while (!Regs.IIR.InterruptID.Pending) {
    if (Regs.IIR.InterruptID.ID == Interrupt::ReadyToSend && counter > 0) {
      counter--;
      WriteByte(0x03F8, TransmissionBuffer[counter]);
      if (counter == 0) {
        DisableInterrupts();
      }
    }
    Regs.IIR.FromInt8(::ReadByte(0x03F8 + 2));
  };
}
SerialPort::SerialPort(Int16 InPort, Int32 InBaudRate, Parity InParity,
                       CharacterLenght Len, StopBit InStopBit)
    : BaudRate(InBaudRate),
      Port(InPort),
      PortParity(InParity),
      CharLength(Len),
      PortStopBit(InStopBit)

{
  DisableInterrupts();
  Regs.LCR.DLAB = true;
  WriteByte(Port + LCROffset, static_cast<UInt8>(Regs.LCR));
  SetBuadRate();

  SetLineControlRegister();
  SetFIFOControlRegister();
  SetModemControlRegister();
  auto *callback = new DelegateOneArg<SerialPort, struct Regs>(
      this, &SerialPort::InterruptHandler);
  InstallIrqHandler(4, callback);
}

SerialPort::~SerialPort() {
  DisableInterrupts();
  UninstallIrqHandler(4);
}

void SerialPort::SetBuadRate() {
  UInt16 buaddivider = PORTCLOCK / BaudRate;
  UInt8 *bd = reinterpret_cast<UInt8 *>(&buaddivider);

  WriteByte(Port, bd[0]);  // TODO: Doesn't work
  WriteByte(Port + 1, bd[1]);
}

void SerialPort::SetModemControlRegister() {
  Regs.MCR.DataTerminalReady = 1;
  Regs.MCR.RequestToSend = 1;
  Regs.MCR.OUT = 2;
  WriteByte(Port + MCROffset, Regs.MCR);
}
void SerialPort::SetFIFOControlRegister() {
  Regs.IIR.FIFOControl.Enable = true;
  Regs.IIR.FIFOControl.RxReset = true;
  Regs.IIR.FIFOControl.TxReset = true;
  Regs.IIR.FIFOControl.RxBuffer = FIFOBuffer::FOURTEEN;
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

bool SerialPort::IsReadyToSend() {
  Regs.LSR.FromInt8(::ReadByte(Port + LSROffset));
  return Regs.LSR.TransmissionEmpty;
}

bool SerialPort::IsDataAvailable() {
  Regs.LSR.FromInt8(::ReadByte(Port + LSROffset));
  return Regs.LSR.DataAvaliable;
}

void SerialPort::Send(char OutByte) {
  TransmissionBuffer[counter++] = OutByte;
  Regs.IER.DataAvailable = false;
  Regs.IER.TransmissionEmpty = true;
  Regs.IER.StatusSignal = false;
  Regs.IER.LineStatus = false;
  WriteByte(Port + IEROffset, Regs.IER);
  // while (!IsReadyToSend()) {
  // };
  // WriteByte(Port, OutByte);
}

void SerialPort::Send(char *OutData, Int32 length) {
  while (!IsReadyToSend()) {
  };
  for (Int32 i = 0; i < length; i++) {
    WriteByte(Port, OutData[i]);
  }
}
char SerialPort::ReadByte() {
  while (!IsDataAvailable()) {
  };
  return ::ReadByte(Port);
}
void SerialPort::ReadData(char *InData, Int32 length) {
  for (Int32 i = 0; i < length; i++) {
    while (!IsDataAvailable()) {
    };
    InData[i] = ::ReadByte(Port);
  }

  return;
}  // We need better API
