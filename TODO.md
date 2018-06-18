
[x] Declare Heap
[x] Format numbers to any base
[ ] Interrupts
[ ] Static array
[ ] KISS Allocator
[ ] Keyboard Input
[ ] Terminal with command line
[ ] File System
[ ] Networking

+ Set up ISRs and trigger manual interrupt
+ Debug via gdb
    + automatic synchronization breakpoint for gdb to get around "g packet reply is too long" problem caused by switching to long mode
    + debug symbols for gdb
    + manually placeable breakpoints


+ had to disable docker because the build caused inconsistencies (because of arch?)
+ Fix IDT related triple fault caused by General Protection Fault (0xD)
+ Bug: PIC remapping was broken due to inline assembler
+ Fix docker build (use correct gcc flags)
+ receive keyboard interrupts
+ acknowledge IRQs
+ Print keyboard key code
+ Debt: remove chaotic debug output
 
-----------------------------------------

- Process keyboard input (e.g. write to terminal)
- Write interrupt handlers for exceptions and irqs and define a convention for them
- Helper to print GDT
- Debt: Move executables to ./bin
- Debt: extend test suite
- Debt: add more diagnostic output (memory regions, stack, IDT, ...)
- Debt: is asm intel_syntax noprefix affecting asm because it is never reset to att_syntax?
- Debt: volatile asm and clobber memory i am not 100% certain of, check e.g. port etc on whether it really is necessary
