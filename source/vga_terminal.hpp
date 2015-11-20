typedef uint8 color;

class VGATerminal {
  static uint16 const ROWS = 25;
  static uint16 const COLUMNS = 80;
  //static uint16* const VGA_MEMORY_ADDRESS = (uint64)0xB8000;

  uint16* const vga_memory;
  color current_color;
  uint16 current_row;
  uint16 current_column;

public:

  VGATerminal() : vga_memory((uint16*)0xB8000) {
    current_color = 2;
    current_row = 1;
    current_column = 0;
  }

  void print(const char * const str) {
    for(uint16 i=0; str[i]; i++) {
      put(current_row, current_column, definePixel(str[i], current_color));
      current_column += 1;
      if(current_column >= COLUMNS) {
        newline();
      }
    }
  }

  void puts(const char * const str) {
    print(str);
    newline();
  }

  void newline() {
    current_row += 1;
    current_column = 0;
  }

  void clear() {
    for(uint16 row = 0; row < ROWS; row++) {
      for(uint16 col = 0; col < COLUMNS; col++) {
        put(row, col, 0);
      }
    }
  }

private:

  uint16 definePixel(char character, color pc) {
    return ((uint16)character) | pc << 8;
  }

  void put(uint16 row, uint16 column, uint16 pixel) {
    vga_memory[row * COLUMNS + column] = pixel;
  }
};
