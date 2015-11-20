#include "types.hpp"
#include "string.hpp"
#include "vga_terminal.hpp"
#include "logger.hpp"

extern "C"
void helvede_kernel_main() {
  VGATerminal t = VGATerminal();
  t.clear();
  t.puts("Sorry, i need some longer text in here dude!\0");
  t.puts("Another string! When will this wrap?\0");
  for(uint16 i = 0; i < 5; i++) {
    t.puts("test\0");
  }
}
