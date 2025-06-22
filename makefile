# Các công cụ bạn sử dụng để biên dịch và build hệ điều hành, tất cả đều dành cho hệ 32-bit.
CC = i686-elf-gcc # Trình biên dịch C cho hệ điều hành 32-bit
AS = i686-elf-as # Trình biên dịch Assembly
LD = i686-elf-ld # Trình liên kết (Linker)
GRUB_MKRESCUE = i686-elf-grub-mkrescue # Tạo ISO bootable

GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

# Clean refactored version objects  
myos = obj/loader.o \
          obj/gdt.o \
          obj/memorymanagement.o \
          obj/drivers/driver.o \
          obj/hardwarecommunication/port.o \
          obj/hardwarecommunication/interruptstubs.o \
          obj/hardwarecommunication/interrupts.o \
          obj/syscalls.o \
          obj/multitasking.o \
          obj/drivers/keyboard.o \
          obj/drivers/vga.o \
          obj/modules/printf.o \
          obj/modules/math_functions.o \
          obj/modules/ui_functions.o \
          obj/modules/app_logic.o \
          obj/kernel.o 

run: myos.iso

qemu-myos: myos.iso
	qemu-system-i386 -cdrom myos.iso -m 128M

# compile cpp to o
obj/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CC) $(GCCPARAMS) -c -o $@ $<

# compile assembly to o
obj/%.o: src/%.s
	mkdir -p $(@D)
	$(AS) $(ASPARAMS) -o $@ $<

# link all o to bin
myos.bin: linker.ld $(myos)
	$(LD) $(LDPARAMS) -T $< -o $@ $(myos)

myos.iso: myos.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp myos.bin iso/boot/myos.bin
	echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "MyOS Clean Refactored" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/myos.bin'    >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) --output=myos.iso iso
	rm -rf iso

.PHONY: clean
clean:
	rm -rf obj myos.bin myos.iso