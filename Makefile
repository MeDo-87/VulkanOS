# Makefile for VulcanOS Kernel


CC = ../cross-compiler/bin/bin/i686-elf-g++
LD = ../cross-compiler/bin/bin/i686-elf-ld

SRC = main.c common.c monitor.c descriptorTables.c isr.c timer.c heap.c
CPPSRC = ctordtor.cpp MemoryManager.cpp keyboard.cpp
ASM = boot.S interrupt.S

SRCDIR = $(PWD)/VulkanOS/source
INCLUDEDIR = $(PWD)/VulkanOS/include
ASMDIR = $(PWD)/VulkanOS/asm

BUILDDIR=$(PWD)/build
OUTDIR=$(BUILDDIR)/bin
TEMPDIR=$(BUILDDIR)/tmp


TARGET = $(OUTDIR)/kernel
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
	$(LD) $(LDFLAGS)  -o $(TARGET)  $(ASMOBJS) $(OBJS) $(CPPOBJS) $(LIBS)

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

updateImage:
	sudo losetup /dev/loop0 ./Testing/Bochs/binary/floppy.img
	sudo mount /dev/loop0 /mnt
	sudo cp $(TARGET) /mnt/kernel
	sudo umount /dev/loop0
	sudo losetup -d /dev/loop0 

run: updateImage
	sudo bochs -qf ./Testing/Bochs/binary/bochsrc.txt
	
clean:
	rm $(TARGET)
	rm $(TEMPDIR)/*.o



