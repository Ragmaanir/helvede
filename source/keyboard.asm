global irq_keyboard_handler
global empty_irq_handler
extern keyboard_handler_main

irq_keyboard_handler:
  mov rax, text_done
  mov qword [0xb8000], rax
  call keyboard_handler_main
  iretd

empty_irq_handler:
  iretd


section .data
  text_done db 'H', 0x0e, 'a', 0x0e, 'l', 0x0e, 't', 0x0e
