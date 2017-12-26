# Makefile for VulcanOS Kernel


CC = ../cross-compiler/bin/bin/i686-elf-g++
LD = ../cross-compiler/bin/bin/i686-elf-ld

SRC = main.c common.c monitor.c descriptorTables.c isr.c timer.c heap.c keyboard.c 
CPPSRC = ctordtor.cpp MemoryManager.cpp

ASM = boot.S interrupt.S

TARGET = kernel
CFLAGS=  -std=c++14 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -pedantic -fno-exceptions -funsigned-char 
CFLAGS+= -ffreestanding -fomit-frame-pointer -mno-red-zone -mno-3dnow -mno-mmx -fno-asynchronous-unwind-tables
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

ASMOBJS = $(ASM:.S=.o)
OBJS = $(SRC:.c=.o)
CPPOBJS = $(CPPSRC:.cpp=.o)

.PHONY: depend clean

all:	kernel
	@echo compiling $(TARGET)

kernel: $(ASMOBJS) $(OBJS) $(CPPOBJS)
	$(LD) $(LDFLAGS)  -o $(TARGET)  $(ASMOBJS) $(OBJS) $(CPPOBJS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ 
.S.o:
	nasm $(ASFLAGS) $< 

clean:
	rm *.o kernel



