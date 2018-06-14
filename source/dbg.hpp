extern "C" void gdb_breakpoint();

namespace Helvede {
  namespace Dbg {
    // TODO: output strings
    void put(uint32 col, uint32 row, char c, uint8 color = 0x2f) {
      ((uint16*)0xB8000)[col + row * 80] = (color << 8) | (uint8) c;
    }

    // set gdb breakpoint
    void breakpoint() {
      put(75, 22, 'B', 0x4f);
      gdb_breakpoint();
    }
  }
}

extern "C" void gdb_breakpoint() {
  // FIXME: need side-effect
  Helvede::Dbg::put(70, 22, ' ', 0x00);
}
