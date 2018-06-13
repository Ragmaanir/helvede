extern empty_isr_handler_callback
extern common_isr_handler_callback

global empty_isr_handler
global common_isr_handler

%define isr_count 256

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
    call empty_isr_handler_callback
    mov rax, [str_zero]
    mov rdi, 0x2f002f002f002f00
    or rax, rdi
    add rax, %1
    mov qword [0xb8000 + 64], rax
    mov rdi, %1
    call common_isr_handler_callback
    isr_pop_registers
    sti
    iretq
%endmacro

%macro concat 2
  %1%2
%endmacro

section .text
bits 64
align 4


; Define all ISR handlers
%assign i 0
%rep isr_count
  define_isr i
%assign i i+1
%endrep


empty_isr_handler:
  cli
  isr_push_registers
  call empty_isr_handler_callback
  isr_pop_registers
  sti
  iretq

section .data
  ;str_empty db 'E', 0x0e, 'm', 0x0e, 't', 0x0e, 'y', 0x0e
  str_OK dd 0x2f, 0x4b, 0x2f, 0x4f
  ;str_zero dq 0x2f, 0x30, 0x2f, 0x30, 0x2f, 0x30, 0x2f, 0x30
  ;str_zero dq 0x30, 0x2f, 0x30, 0x2f, 0x30, 0x2f, 0x30, 0x2f
  str_zero dq '0', 0x2f, '0', 0x2f, '0', 0x2f, '0', 0x2f
