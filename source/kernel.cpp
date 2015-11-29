#include "types.hpp"
#include "math.hpp"
#include "numbers.hpp"
#include "memory_manager.hpp"
#include "bounded_string.hpp"
#include "vga_terminal.hpp"
#include "logger.hpp"


extern "C"
void helvede_kernel_main() {
  VGATerminal t = VGATerminal();
  t.clear();
  t.puts("Welcome to Helvede");
}
