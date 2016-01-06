# Makefile for VulcanOS Kernel


CC = ../cross-compiler/bin/bin/i686-elf-gcc
LD = ../cross-compiler/bin/bin/i686-elf-ld

SRC = main.c common.c monitor.c descriptorTables.c isr.c timer.c keyboard.c

ASM = boot.S interrupt.S

TARGET = kernel
CFLAGS= -nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

ASMOBJS = $(ASM:.S=.o)
OBJS = $(SRC:.c=.o)

.PHONY: depend clean

all:	kernel
	@echo compiling $(TARGET)

kernel: $(ASMOBJS) $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJS) $(ASMOBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ 
.S.o:
	nasm $(ASFLAGS) $< 

clean:
	rm *.o kernel



