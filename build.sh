rm -r output
mkdir -p output/objects
mkdir -p output/image/boot/grub
g++ -std=c++11 -c source/kernel.cpp -o output/objects/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-stack-protector
nasm -f elf64 source/boot_header.asm -o output/objects/boot_header.o &&
nasm -f elf64 source/boot_long_mode.asm -o output/objects/boot_long_mode.o &&
nasm -f elf64 source/boot.asm -o output/objects/boot.o &&
ld -n -o output/image/boot/kernel.bin -T source/linker.ld output/objects/boot_header.o output/objects/boot.o output/objects/boot_long_mode.o output/objects/kernel.o &&
cp source/grub.cfg output/image/boot/grub/ &&
grub-mkrescue -o output/image.iso output/image
