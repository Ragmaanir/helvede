global helvede_kernel_long_mode

section .text
bits 64
helvede_kernel_long_mode:
  cli
  call call_helvede_kernel_main
  jmp permanent_halt

call_helvede_kernel_main:
  ; initialize stack
  extern helvede_kernel_stack_top
  mov rsp, helvede_kernel_stack_top

  extern helvede_kernel_heap_start
  ; NOTE: rdi is used on Linux, rcx on Windows
  mov rdi, helvede_kernel_heap_start

  push rbp
  extern helvede_kernel_main
  call helvede_kernel_main
  pop rbp

  ret

permanent_halt:
  hlt
  jmp permanent_halt

print:
  mov qword [0xb8000], rax
  ret

fatal_error:
  call print
  hlt

; section .data
;   text_done db 'H', 0x0e, 'a', 0x0e, 'l', 0x0e, 't', 0x0e
;   text_kernel_main db 'M', 0x2f, 'a', 0x2f, 'i', 0x2f, 'n', 0x2f
