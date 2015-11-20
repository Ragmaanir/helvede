global helvede_kernel_long_mode

section .text
bits 64
helvede_kernel_long_mode:
  ; print `OKAY` to screen
  mov rax, 0x2f592f412f4b2f4f
  mov qword [0xb8000], rax
  cli
  extern helvede_kernel_main
  call helvede_kernel_main
  mov eax, [text_done]
  call print
  hlt

print:
  mov dword [0xb8000], eax
  ret

fatal_error:
  call print
  hlt

section .data
  ;text_hello dd 0x2f4b2f4f
  text_done db 'X', 0x2f, 'X', 0x2f
