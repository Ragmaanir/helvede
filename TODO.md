
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

- Helper to print GDT
- Fix IDT related triple fault caused by General Protection Fault (0xD)
- Create handler for IRQs and trigger it via keyboard
- Move executables to ./bin
- Fix docker build
- Debt: remove chaotic debug output
- Debt: extend test suite
- Debt: add more diagnostic output (memory regions, stack, IDT, ...)
