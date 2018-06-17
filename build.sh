rm -r output
mkdir -p output/objects
mkdir -p output/image/boot/grub
echo "--- g++ ---" &&
g++ -c source/kernel.cpp -o output/objects/kernel.o @gcc_options
#clang-3.6 -std=c++14 -m64 -masm=intel -c source/kernel.cpp -o output/objects/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-stack-protector -fno-use-cxa-atexit -Wno-gnu-array-member-paren-init -Wno-unused-parameter
echo "--- NASM ---" &&
nasm -f elf64 -g -F dwarf source/boot_header.asm -o output/objects/boot_header.o &&
nasm -f elf64 -g -F dwarf source/boot_long_mode.asm -o output/objects/boot_long_mode.o &&
nasm -f elf64 -g -F dwarf source/boot.asm -o output/objects/boot.o &&
nasm -f elf64 -g -F dwarf source/interrupts.asm -o output/objects/interrupts.o &&
echo "--- Linking ---" &&
ld -m elf_x86_64 -n -nostdlib -o output/image/boot/kernel.bin -T source/linker.ld output/objects/boot_header.o output/objects/boot.o output/objects/boot_long_mode.o output/objects/interrupts.o output/objects/kernel.o &&
cp source/grub.cfg output/image/boot/grub/ &&
grub-mkrescue -o output/image.iso output/image
