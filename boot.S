MBOOT_PAGE_ALIGN equ 1<<0	;load on page boundary
MBOOT_MEM_INFO equ 1<<1		;Provide memory info
MBOOT_HEADER_MAGIC_NUMBER equ 0x1BADB002	;Multiboot Magic Number

MBOOT_HEADER_FLAGS equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM	equ -(MBOOT_HEADER_MAGIC_NUMBER + MBOOT_HEADER_FLAGS)

[BITS 32]		;32 bit instructions
[GLOBAL mboot]		;"mboot" accessible from C
;[EXTERN code]		;Start the .text section
;[EXTERN bss]		;Start the .bss section
;[EXTERN end]		;End of last loadable section

mboot:
  dd MBOOT_HEADER_MAGIC_NUMBER	;Grub will search for this value on each 4byte boundry
  dd MBOOT_HEADER_FLAGS
  dd MBOOT_CHECKSUM
  ;dd mboot
  ;dd code
  ;dd bss
  ;dd end
  ;dd start

SECTION  .bss
stack_bottom:
    resb 16384 ; 16 KiB
stack_top:
SECTION  .text

[GLOBAL start]		;Entry point
[EXTERN main]		;Entry to c

start:
  push ebx		; load multiboot header location
  cli
  call main
  jmp $
  startSize: equ $ - start
