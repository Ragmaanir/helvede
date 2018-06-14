global helvede_kernel_long_mode
global gdb_synchronization

section .text
bits 64
helvede_kernel_long_mode:
  cli
  call call_helvede_kernel_main
  jmp permanent_halt

call_helvede_kernel_main:
  call wait_for_gdb

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

  jmp permanent_halt

; wait until condition is changed via GDB to prevent bug "g packet reply is too long"
; see: https://stackoverflow.com/questions/8662468/remote-g-packet-reply-is-too-long
wait_for_gdb:
  mov rax, 0

  .loop_start:
    mov rbx, [text_waiting_for_gdb]
    mov qword [0xb8000 + 0x100], rbx

    call gdb_synchronization
    test rax, rax
    jz .loop_start

  ret

; This breakpoint is called from the wait_for_gdb-loop. Once GDB is connected
; to QEMU the breakpoint is triggered. The wait_for_gdb-loop can then be skipped
; by executing "set $rax = 1" in GDB and continuing.
gdb_synchronization:
  ; mov rbx, [text_gdb_breakpoint_reached]
  ; mov qword [0xb8000 + 0x108], rbx
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

section .data
  text_waiting_for_gdb db 'W', 0x4f, 'A', 0x4f, 'I', 0x4f, 'T', 0x4f
  text_gdb_breakpoint_reached db 'X', 0x0f, 'X', 0x0f, 'X', 0x0f, 'X', 0x0f
  text_done_waiting db 'G', 0x2f, 'D', 0x2f, 'B', 0x2f, '!', 0x2f

; section .data
;   text_done db 'H', 0x0e, 'a', 0x0e, 'l', 0x0e, 't', 0x0e
;   text_kernel_main db 'M', 0x2f, 'a', 0x2f, 'i', 0x2f, 'n', 0x2f
