%macro ISR_NOERRCODE 1  ; define a macro, taking one parameter
  [GLOBAL isr%1]        ; %1 accesses the first parameter.
  isr%1:
    cli
    push byte 0
    push byte %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
  [GLOBAL isr%1]
  isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro 

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

%macro IRQ_CODE 2	;Macro for IRQ codes
  [GLOBAL irq%1]
  irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro 

IRQ_CODE 0, 32
IRQ_CODE 1, 33
IRQ_CODE 2, 34
IRQ_CODE 3, 35
IRQ_CODE 4, 36
IRQ_CODE 5, 37
IRQ_CODE 6, 38
IRQ_CODE 7, 39
IRQ_CODE 8, 40
IRQ_CODE 9, 41
IRQ_CODE 10, 42
IRQ_CODE 11, 43
IRQ_CODE 12, 44
IRQ_CODE 13, 45
IRQ_CODE 14, 46
IRQ_CODE 15, 47

[EXTERN isrHandler]	;External handler defined in isr.c

isr_common_stub:
   pusha	;pushes all GP registers
   mov ax, ds	;ax = ds
   push eax;

   mov ax, 0x10
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax	
   call isrHandler

   pop eax	; reload the original data segment
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax
   popa
   add esp, 8
   sti
   iret

[EXTERN irqHandler]

irq_common_stub:
   pusha	;pushes all GP registers
   mov ax, ds	;ax = ds
   push eax;

   mov ax, 0x10
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax	
   call irqHandler

   pop eax	; reload the original data segment
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   popa
   add esp, 8
   sti

   iret
