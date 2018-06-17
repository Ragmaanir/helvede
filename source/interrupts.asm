extern empty_isr_handler_callback
extern common_isr_handler_callback

extern gdb_breakpoint

global empty_isr_handler
global common_isr_handler
global isr_pointer_table

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
  core_isr_%1:
    xchg bx, bx
    cli
    isr_push_registers
    ; ;
    ; mov rax, [str_zero]
    ; mov rdi, 0x5f005f005f005f00
    ; or rax, rdi
    ; add rax, %1
    ; mov qword [0xb8000 + 140], rax
    ; ;

    ;mov rdi, %1
    ;call print_isr_number

    mov rdi, %1
    call common_isr_handler_callback

    isr_pop_registers
    sti
    iretq
%endmacro

; %macro concat 2
;   %1%2
; %endmacro

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

print_isr_number:
  mov ax, di
  or ax, 0x6f00
  mov word [0xb8000 + 140], ax
  ret

; print_isr_number:
;   mov cx, di
;   add cx, 48
;   or cx, 0x2f00
;   mov word [0xb8000 + 130], cx

;   mov rax, 0x6f006f006f006f00
;   shl rdi, 48
;   or rax, rdi
;   mov qword [0xb8000 + 140], rax
;   ret

%define  core_isr_ref(i) core_isr_%+i

section .data
  ;str_empty db 'E', 0x0e, 'm', 0x0e, 't', 0x0e, 'y', 0x0e
  ;str_OK dd 0x2f, 0x4b, 0x2f, 0x4f
  ;str_zero dq 0x2f, 0x30, 0x2f, 0x30, 0x2f, 0x30, 0x2f, 0x30
  ;str_zero dq 0x30, 0x2f, 0x30, 0x2f, 0x30, 0x2f, 0x30, 0x2f
  ;str_zero dq '0', 0x2f, '0', 0x2f, '0', 0x2f, '0', 0x2f
  ;zero dq 0

  ; A table of all interrupt service routines
  ; ----
  ; This table is exported to c++ so one does not have
  ; to have lists of function references in c++.
  ; This is done here since NASM macros are more powerful
  ; than c++ preprocessor (repetition is easy in NASM).
  isr_pointer_table:
    %assign i 0
    %rep isr_count
      dq core_isr_ref(i)
    %assign i i+1
    %endrep
