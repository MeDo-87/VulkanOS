MBOOT_PAGE_ALIGN equ 1<<0	;load on page boundary
MBOOT_MEM_INFO equ 1<<1		;Provide memory info
MBOOT_HEADER_MAGIC_NUMBER equ 0x1BADB002	;Multiboot Magic Number

MBOOT_HEADER_FLAGS equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM	equ -(MBOOT_HEADER_MAGIC_NUMBER + MBOOT_HEADER_FLAGS)

[BITS 32]		;32 bit instructions
[GLOBAL mboot]		;"mboot" accessible from C
[EXTERN code]		;Start the .text section
[EXTERN bss]		;Start the .bss section
[EXTERN end]		;End of last loadable section

mboot:
  dd MBOOT_HEADER_MAGIC_NUMBER	;Grub will search for this value on each 4byte boundry
  dd MBOOT_HEADER_FLAGS
  dd MBOOT_CHECKSUM
  dd mboot
  dd code
  dd bss
  dd end
  dd start

[GLOBAL start]		;Entry point
[EXTERN main]		;Entry to c


; This will set up our new segment registers. We need to do
; something special in order to set CS. We do what is called a
; far jump. A jump that includes a segment as well as an offset.
; This is declared in C as 'extern void gdt_flush();'
[GLOBAL gdtFlush]     ; Allows the C code to link to this

gdtFlush:
    mov eax, [esp + 4]
    lgdt [eax]        ; Load the GDT with our '_gp' which is a special pointer
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2   ; 0x08 is the offset to our code segment: Far jump!
flush2:
    ret               ; Returns back to the C code!

; Loads the IDT passed as argument in EAX
[GLOBAL idtLoad]
idtLoad:
    mov eax, [esp + 4]
    lidt [eax]
    ret

	
start:
  push ebx		; load multiboot header location
  cli
  call main
  jmp $
