extern keyboard_isr_handler_callback
extern empty_isr_handler_callback
extern common_isr_handler_callback

global keyboard_isr_handler
global empty_isr_handler
global common_isr_handler

%macro isr_push_registers 0
  push rax
  push rcx
  push rdx
  push rbx
  push rsp
  push rbp
  push rsi
  push rdi
%endmacro

%macro isr_pop_registers 0
  pop rdi
  pop rsi
  pop rbp
  pop rsp
  pop rbx
  pop rdx
  pop rcx
  pop rax
%endmacro

%macro define_isr 1
  global core_isr_%1

  core_isr_%1:
    xchg bx, bx
    cli
    isr_push_registers
    push %1
    call common_isr_handler
    isr_pop_registers
    sti
    iretq
%endmacro

section .text
bits 64
align 4

; Define all ISR handlers
%assign i 0
%rep 34
  define_isr i
%assign i i+1
%endrep

common_isr_handler:
  call common_isr_handler_callback

keyboard_isr_handler:
  xchg bx, bx
  cli
  isr_push_registers
  mov rax, 0x2f4b2f4f
  mov qword [0xb8000], rax
  call keyboard_isr_handler_callback
  isr_pop_registers
  sti
  iretq

empty_isr_handler:
  cli
  isr_push_registers
  ;mov rax, text_done
  mov rax, 0x2f4b2f4f
  mov qword [0xb8000], rax
  call empty_isr_handler_callback
  isr_pop_registers
  sti
  iretq

section .data
  ;str_empty db 'E', 0x0e, 'm', 0x0e, 't', 0x0e, 'y', 0x0e
  str_OK dd 0x2f, 0x4b, 0x2f, 0x4f
