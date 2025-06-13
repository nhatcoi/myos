# sudo apt-get install g++ binutils libc6-dev-i386
# sudo apt-get install VirtualBox grub-legacy xorriso

CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-ld
GRUB_MKRESCUE = i686-elf-grub-mkrescue

GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
          obj/gdt.o \
          obj/memorymanagement.o \
          obj/drivers/driver.o \
          obj/hardwarecommunication/port.o \
          obj/hardwarecommunication/interruptstubs.o \
          obj/hardwarecommunication/interrupts.o \
          obj/syscalls.o \
          obj/multitasking.o \
          obj/drivers/keyboard.o \
          obj/drivers/mouse.o \
          obj/drivers/vga.o \
          obj/gui/widget.o \
          obj/gui/desktop.o \
          obj/gui/window.o \
          obj/kernel.o

# Clean refactored version objects  
objects_clean = obj/loader.o \
          obj/gdt.o \
          obj/memorymanagement.o \
          obj/drivers/driver.o \
          obj/hardwarecommunication/port.o \
          obj/hardwarecommunication/interruptstubs.o \
          obj/hardwarecommunication/interrupts.o \
          obj/syscalls.o \
          obj/multitasking.o \
          obj/drivers/keyboard.o \
          obj/drivers/mouse.o \
          obj/drivers/vga.o \
          obj/gui/widget.o \
          obj/gui/desktop.o \
          obj/gui/window.o \
          obj/modules/printf.o \
          obj/modules/math_functions.o \
          obj/modules/ui_functions.o \
          obj/modules/app_logic.o \
          obj/kernel_clean.o

# Debug version objects
debug_objects = obj/loader.o \
          obj/gdt.o \
          obj/memorymanagement.o \
          obj/drivers/driver.o \
          obj/hardwarecommunication/port.o \
          obj/hardwarecommunication/interruptstubs.o \
          obj/hardwarecommunication/interrupts.o \
          obj/syscalls.o \
          obj/multitasking.o \
          obj/drivers/keyboard.o \
          obj/drivers/mouse.o \
          obj/drivers/vga.o \
          obj/gui/widget.o \
          obj/gui/desktop.o \
          obj/gui/window.o \
          obj/kernel_debug.o



run: mykernel.iso
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm 'My Operating System' &

qemu: mykernel.iso
	qemu-system-i386 -cdrom mykernel.iso -m 128M

qemu-clean: mykernel_clean.iso
	qemu-system-i386 -cdrom mykernel_clean.iso -m 128M

qemu-debug: mykernel.iso
	qemu-system-i386 -cdrom mykernel.iso -m 128M -s -S

debug: mykernel_debug.iso
	qemu-system-i386 -cdrom mykernel_debug.iso -m 128M

mykernel_debug.bin: linker.ld $(debug_objects)
	$(LD) $(LDPARAMS) -T $< -o $@ $(debug_objects)

mykernel_debug.iso: mykernel_debug.bin
	mkdir iso_debug
	mkdir iso_debug/boot
	mkdir iso_debug/boot/grub
	cp mykernel_debug.bin iso_debug/boot/mykernel.bin
	echo 'set timeout=0'                      > iso_debug/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso_debug/boot/grub/grub.cfg
	echo ''                                  >> iso_debug/boot/grub/grub.cfg
	echo 'menuentry "Debug OS" {' >> iso_debug/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'    >> iso_debug/boot/grub/grub.cfg
	echo '  boot'                            >> iso_debug/boot/grub/grub.cfg
	echo '}'                                 >> iso_debug/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) --output=mykernel_debug.iso iso_debug
	rm -rf iso_debug

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CC) $(GCCPARAMS) -c -o $@ $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	$(AS) $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	$(LD) $(LDPARAMS) -T $< -o $@ $(objects)

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp mykernel.bin iso/boot/mykernel.bin
	echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'    >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) --output=mykernel.iso iso
	rm -rf iso

mykernel_clean.bin: linker.ld $(objects_clean)
	$(LD) $(LDPARAMS) -T $< -o $@ $(objects_clean)

mykernel_clean.iso: mykernel_clean.bin
	mkdir iso_clean
	mkdir iso_clean/boot
	mkdir iso_clean/boot/grub
	cp mykernel_clean.bin iso_clean/boot/mykernel.bin
	echo 'set timeout=0'                      > iso_clean/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso_clean/boot/grub/grub.cfg
	echo ''                                  >> iso_clean/boot/grub/grub.cfg
	echo 'menuentry "MyOS Clean Refactored" {' >> iso_clean/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'    >> iso_clean/boot/grub/grub.cfg
	echo '  boot'                            >> iso_clean/boot/grub/grub.cfg
	echo '}'                                 >> iso_clean/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) --output=mykernel_clean.iso iso_clean
	rm -rf iso_clean

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

.PHONY: clean
clean:
	rm -rf obj mykernel.bin mykernel.iso mykernel_clean.bin mykernel_clean.iso mykernel_debug.bin mykernel_debug.iso

# Build targets
.PHONY: original clean-refactored all-versions
original: mykernel.iso
clean-refactored: mykernel_clean.iso
all-versions: original clean-refactored
