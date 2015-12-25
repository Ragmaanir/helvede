global helvede_kernel_boot
global helvede_kernel_heap_start

extern helvede_kernel_long_mode

section .text
bits 32
align 4
helvede_kernel_boot:
  ;mov esp, stack_top ; initialize stack pointer
  ;call initialize_vesa
  call test_multiboot
  call test_cpuid
  call test_long_mode
  call setup_page_tables
  call enable_paging
  lgdt [gdt64.pointer]
  ; update selectors
  mov ax, gdt64.data
  mov ss, ax  ; stack selector
  mov ds, ax  ; data selector
  mov es, ax  ; extra selector

  mov edi, helvede_kernel_heap_start
  jmp gdt64.code:helvede_kernel_long_mode
  hlt

print:
  mov dword [0xb8000], eax
  ret

fatal_error:
  call error_code
  call print
  jmp permanent_halt

permanent_halt:
  hlt
  jmp permanent_halt

error_code:
  add eax, "0"
  or eax, 0x2f002f00
  ret

test_multiboot:
  cmp eax, 0x36d76289
  jne .no_multiboot
  ret
.no_multiboot:
  mov eax, 0
  jmp fatal_error

test_cpuid:
  pushfd               ; Store the FLAGS-register.
  pop eax              ; Restore the A-register.
  mov ecx, eax         ; Set the C-register to the A-register.
  xor eax, 1 << 21     ; Flip the ID-bit, which is bit 21.
  push eax             ; Store the A-register.
  popfd                ; Restore the FLAGS-register.
  pushfd               ; Store the FLAGS-register.
  pop eax              ; Restore the A-register.
  push ecx             ; Store the C-register.
  popfd                ; Restore the FLAGS-register.
  xor eax, ecx         ; Do a XOR-operation on the A-register and the C-register.
  jz .no_cpuid         ; The zero flag is set, no CPUID.
  ret                  ; CPUID is available for use.
.no_cpuid:
  mov eax, 1
  jmp fatal_error

test_long_mode:
  mov eax, 0x80000000    ; Set the A-register to 0x80000000.
  cpuid                  ; CPU identification.
  cmp eax, 0x80000001    ; Compare the A-register with 0x80000001.
  jb .no_long_mode       ; It is less, there is no long mode.
  mov eax, 0x80000001    ; Set the A-register to 0x80000001.
  cpuid                  ; CPU identification.
  test edx, 1 << 29      ; Test if the LM-bit, which is bit 29, is set in the D-register.
  jz .no_long_mode       ; They aren't, there is no long mode.
  ret
.no_long_mode:
  mov eax, 2
  jmp fatal_error

setup_page_tables:
  ; map first P4 entry to P3 table
  mov eax, p3_table
  or eax, 0b11 ; present + writable
  mov [p4_table], eax

  ; map first P3 entry to P2 table
  mov eax, p2_table
  or eax, 0b11 ; present + writable
  mov [p3_table], eax

  ; map each P2 entry to a huge 2MiB page
  mov ecx, 0

.map_p2_table:
  ; map ecx-th P2 entry to a huge page that starts at address 2MiB*ecx
  mov eax, 0x200000  ; 2MiB
  mul ecx            ; start address of ecx-th page
  or eax, 0b10000011 ; present + writable + huge
  mov [p2_table + ecx * 8], eax ; map ecx-th entry

  inc ecx            ; increase counter
  cmp ecx, 512       ; if counter == 512, the whole P2 table is mapped
  jne .map_p2_table  ; else map the next entry

  ret

enable_paging:
  ; load P4 to cr3 register (cpu uses this to access the P4 table)
  mov eax, p4_table
  mov cr3, eax

  ; enable PAE-flag in cr4 (Physical Address Extension)
  mov eax, cr4
  or eax, 1 << 5
  mov cr4, eax

  ; set the long mode bit in the EFER MSR (model specific register)
  mov ecx, 0xC0000080
  rdmsr
  or eax, 1 << 8
  wrmsr

  ; enable paging in the cr0 register
  mov eax, cr0
  or eax, 1 << 31
  or eax, 1 << 16
  mov cr0, eax

  ret

initialize_vesa:
  mov  bx,4112h
  mov  ax,4f01h
  ;mov  di,Mode_Info
  mov  cx,bx
  int  10h

  mov  ax,4f02h
  int  10h
  ret

initialize_vesa_custom:
  mov ax, 4f02h
  mov bx, 100h
  ;mov bx, 11bh ; 1,280×1,024
  int 10h

section .data
  ;text_hello dd 0x2f4b2f4f
  text_done db 'X', 0x2f, 'X', 0x2f

section .rodata
gdt64:
  dq 0 ; zero entry
.code: equ $ - gdt64 ; new
  dq (1<<44) | (1<<47) | (1<<41) | (1<<43) | (1<<53) ; code segment
.data: equ $ - gdt64 ; new
  dq (1<<44) | (1<<47) | (1<<41) ; data segment
.pointer:
  dw $ - gdt64 - 1
  dq gdt64

section .bss
align 4096
p4_table:
  resb 4096
p3_table:
  resb 4096
p2_table:
  resb 4096
stack_bottom:
  align 4
  resb 16384
stack_top:
helvede_kernel_heap_start:
