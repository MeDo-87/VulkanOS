# Makefile for VulcanOS Kernel


CC = ../cross-compiler/bin/bin/i686-elf-g++
LD = ../cross-compiler/bin/bin/i686-elf-ld

SRC = main.c common.c monitor.c descriptorTables.c isr.c timer.c heap.c keyboard.c 
CPPSRC = ctordtor.cpp MemoryManager.cpp
ASM = boot.S interrupt.S

SRCDIR = $(PWD)/VulkanOS/source
INCLUDEDIR = $(PWD)/VulkanOS/include
ASMDIR = $(PWD)/VulkanOS/asm

BUILDDIR=$(PWD)/build
OUTDIR=$(BUILDDIR)/bin
TEMPDIR=$(BUILDDIR)/tmp


TARGET = kernel
CFLAGS=  -std=c++14 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -pedantic -fno-exceptions -funsigned-char 
CFLAGS+= -ffreestanding -fomit-frame-pointer -mno-red-zone -mno-3dnow -mno-mmx -fno-asynchronous-unwind-tables
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

ASMOBJS = $(patsubst %.S,$(TEMPDIR)/%.o,$(ASM))
OBJS = $(patsubst %.c, $(TEMPDIR)/%.o,$(SRC))
CPPOBJS = $(patsubst %.cpp, $(TEMPDIR)/%.o,$(CPPSRC))
MKDIR_P = mkdir -p


.PHONY: depend clean

all:	directories kernel
		@echo compiling $(TARGET)

kernel: $(ASMOBJS) $(OBJS) $(CPPOBJS)
	$(LD) $(LDFLAGS)  -o $(OUTDIR)/$(TARGET)  $(ASMOBJS) $(OBJS) $(CPPOBJS) $(LIBS)

$(TEMPDIR)/%.o:$(SRCDIR)/%.cpp
	$(CC) $(CFLAGS)  -I$(INCLUDEDIR) -c $< -o $@

$(TEMPDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) -c $< -o $@

$(TEMPDIR)/%.o:$(ASMDIR)/%.S
	nasm $(ASFLAGS) $< -o $@

directories: $(OUTDIR) $(TEMPDIR)

$(OUTDIR):
	$(MKDIR_P) $(OUTDIR)

$(TEMPDIR):
	$(MKDIR_P) $(TEMPDIR)

clean:
	rm *.o kernel



