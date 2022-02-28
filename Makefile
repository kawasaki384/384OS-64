MAKEFILE_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
LIBDIR = $(MAKEFILE_DIR)Library
INCDIR = $(MAKEFILE_DIR)/src/include
TARGET = kernel.elf
KERNEL = kernel

#もしリンクのエラーが出たらファイルを書く順番に原因があるかもしれないのでチェックしとく。
#新しくCファイルを追加するときは、OBJSとDIROBJS,CFILESに入力する。
OBJS = AsmMain.o gdt_func.o paging_func.o interrupt_func.o segment.o io.o serialport.o \
			paging.o memmgr.o local_apic.o l_apic_timer.o interrupt.o main.o newlib_support.o #objectのファイル

DIROBJS = objs/AsmMain.o objs/gdt_func.o objs/interrupt_func.o objs/paging_func.o objs/segment.o objs/io.o objs/serialport.o \
			objs/paging.o objs/memmgr.o objs/local_apic.o objs/l_apic_timer.o objs/interrupt.o objs/main.o objs/newlib_support.o  #objs/objectのファイル

CFILES = io.c serialport.c segment.c paging.o memmgr.c local_apic.c l_apic_timer.c interrupt.c main.c newlib_support.c #Cのファイル
ASMFILES = AsmMain.asm gdt_func.asm	paging_func.asm iterrupt_func.asm

CC = x86_64-elf-gcc #Compiler
#compiler example
#x86_64-elf-gcc
#i386-elf-gcc
#i686-elf-gcc
#aarch64-elf-gcc
#powerpc-elf-gcc

OBJDIR = objs #ObjectDirectry

CAPSELLA_RELEASE_DIR = capsella/target/release #capsella_release_directry
CAPSELLA_INCLUDE_DIR = capsella_lib/include #capsella_include_direvtry

#capsella staticlibrary
CAPSELLA_LIB = capsella_lib/target/release/libcapsella_lib.a
#capsella headerfile
CAPSELLA_LIB_H = capsella_lib/include/test_header.h

#C CompileOptions
CFLAGS = -I$(MAKEFILE_DIR)/$(KERNEL)/include -I$(LIBDIR)/include -I$(LIBDIR)include/freetype2 -I$(MAKEFILE_DIR)../edk2/MdePkg/Include -I$(CAPSELLA_INCLUDE_DIR) \
-I$(MAKEFILE_DIR)../edk2/MdePkg/Include/X64  -D__ELF__   -D_LDBL_EQ_DBL   -D_GNU_SOURCE   -D_POSIX_TIMERS  -DEFIAPI='__attribute__((ms_abi))' \
-O2 -Wall -g  -ffreestanding -mno-red-zone -mgeneral-regs-only -c -lm 

#LLVM CompileOptions
LDFLAGS = -L$(LIBDIR)/lib -Lcapsella_lib/target/release --entry KernelMain -lc -lm -lg -lcapsella_lib -z norelro --image-base 0x100000 --static

ASMFLAGS = -f elf64 -o 

#make all
.PHONY: all
all: $(TARGET)

kernel.elf: $(OBJS) $(CAPSELLA_LIB) $(CAPSELLA_LIB_H) Makefile
	ld.lld $(LDFLAGS) -o kernel.elf $(DIROBJS)

#make Cfile(in src directry
%.o: $(KERNEL)/%.c Makefile
	$(CC) $(CFLAGS) -c -o objs/$@ $<
#make Cfile(in src directry

#make ioport
%.o: $(KERNEL)/io/%.c Makefile
	$(CC) $(CFLAGS) -c -o objs/$@ $<
#make ioport

#make assembly files
%.o: $(KERNEL)/%.asm Makefile
	nasm $(ASMFLAGS) objs/$@ $<
#make assembly files

#make driver
#make serialport driver
%.o: $(KERNEL)/driver/serialport/%.c Makefile
	$(CC) $(CFLAGS) -c -o objs/$@ $<
#make serialport driver

#make localapic driver
%.o: $(KERNEL)/driver/localapic/%.c Makefile
	$(CC) $(CFLAGS) -c -o objs/$@ $<

#make l_apic_timer driver
%.o: $(KERNEL)/driver/localapic/timer/%.c Makefile
	$(CC) $(CFLAGS) -c -o objs/$@ $<
#make l_apic_timer driver

#make l_apic driver

#make driver

#make gdt
#make gdt_asm
%.o: $(KERNEL)/gdt/%.asm Makefile
	nasm $(ASMFLAGS) objs/$@ $<
#make gdt_asm

#make gdt_c
%.o: $(KERNEL)/gdt/%.c Makefile
	$(CC) $(CFLAGS) -c -o objs/$@ $<
#make gdt_c
#make gdt

#make paging
#make paging_asm
%.o: $(KERNEL)/paging/%.asm Makefile
	nasm $(ASMFLAGS) objs/$@ $<
#make paging_asm

#make paging_c
%.o: $(KERNEL)/paging/%.c Makefile
	$(CC) $(CFLAGS) -c -o objs/$@ $<
#make paging_c
#make paging

#make memmgr
%.o: $(KERNEL)/mem/%.c Makefile
	$(CC) $(CFLAGS) -c -o objs/$@ $<
#make memmgr

#make idt
#make idt_c
%.o: $(KERNEL)/idt/%.c Makefile
	$(CC) $(CFLAGS) -c -o objs/$@ $<
#make idt_c

#make idt_asm
%.o: $(KERNEL)/idt/%.asm Makefile
	nasm $(ASMFLAGS) objs/$@ $<

#make capsella
#build_capsella_lib
%.a: capsella_lib/src/lib.rs Makefile
	cd capsella_lib&&cargo b --release 
#build capsella_lib

#build capsella_header
%.h: capsella_lib/src/lib.rs Makefile
	cd capsella_lib && \
	cbindgen \
	--config cbindgen.toml \
	--crate capsella_lib \
	--output include/test_header.h \
	--lang c
#build capsella_header
#make capsella

#cross-compiler install
.PHONY: CCInstall
CCInstall:
	sudo bash Installers/CCDownload.sh

#make clean
.PHONY: clean
clean:
	rm -rf objs/*.o

#make run
.PHONY: run
run:
	make
	cp kernel.elf 384OS_test/root/kernel.elf
	cd 384OS_test && bash run-qemu.sh
	qemu-system-x86_64   -bios /usr/share/ovmf/OVMF.fd   -drive format=raw,file=fat:rw:root

