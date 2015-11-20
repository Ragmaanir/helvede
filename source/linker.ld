OUTPUT_FORMAT(elf32-i386)
ENTRY(helvede_kernel_boot)

SECTIONS {
  . = 1M;
  .boot : { *(.boot_header) } /* ensure that the multiboot header is at the beginning */
  .text : { *(.text) }
  .data : { *(.data) }
  .bss :  { *(.bss) }
}
