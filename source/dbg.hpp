namespace Helvede {
  namespace Dbg {
    void put(uint32 col, uint32 row, char c, uint8 color = 0x2f) {
      ((uint16*)0xB8000)[col + row * 80] = (color << 8) | (uint8) c;
    }
  }
}
