export AS = nasm
export ASFLAGS = -f elf -o
export CC = gcc
#export CC = /home/davide/cross/bin/i386-elf-gcc
export CFLAGS = -c -Wall -g -gstabs -Wextra -fno-builtin -nodefaultlibs -nostartfiles -nostdlib -m32 -I $(PWD)/include
#export CFLAGS = -c -Wall -g -Wextra -I $(PWD)/include
export LD = ld
#export LD = /home/davide/cross/bin/i386-elf-ld
export LDFLAGS = -m elf_i386 -T linker.ld

all:
	cd apps; make
	cd bootloader; make
	cd drivers; make
	cd fs; make
	cd lib; make
	cd hal; make
	cd mm; make
	cd proc; make
	
	cd apps/example; make
	cd apps/editor; make
	cd apps/multiprocessing; make

	$(CC) $(CFLAGS) main.c
	$(LD) $(LDFLAGS) -o kernel.bin \
	apps/*.o \
	bootloader/*.o \
	drivers/*.o \
	drivers/*/*.o \
	fs/*.o \
	lib/*.o \
	hal/*.o \
	mm/*.o \
	proc/*.o \
	*.o

	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o hoho.iso iso
run:
	VBoxManage startvm Hoho

qemu:
	qemu-system-i386 -d in_asm -hda hdd.img -fda floppy.img -kernel kernel.bin

clean:
	@find . \( -name '*.o' \) -print -exec rm -f '{}' \;
	rm -rf iso hoho.iso kernel.bin

