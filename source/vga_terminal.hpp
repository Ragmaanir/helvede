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

  enum class VGAColor : uint8 {
    Green = 0x2,
    LightGreen = 0xa,
    Gray = 0x7,
    Marine = 0x3,
    DarkGray = 0x8,
    Yellow = 0x9,
    LightYellow = 0xe,
    Orange = 0x6,
    Red = 0x4,
    LightRed = 0xc,
    DarkBlue = 0x1,
    Blue = 0x9,
    LightBlue = 0xb,
    Pink = 0x5,
    LightPink = 0xd,
    White = 0xf,
    Black = 0
  };

  VGATerminal() : vga_memory((uint16*)0xB8000) {
    current_color = 0x02;
    current_row = 0;
    current_column = 0;
  }

  template<class Size>
  void puts(BoundedString<Size> &string) {
    print(string);
    newline();
  }

  template<class Size>
  void print(BoundedString<Size> &string) {
    uint32 col = current_column;

    for(Size i=0; i < string.length(); i++) {
      put(current_row, col, definePixel(string.rawString()[i], current_color));
      col += 1;
      if(col >= COLUMNS) {
        col = 0;
        newline();
      }
    }

    current_column = col;
  }

  void print(RawString str) {
    print(str, current_color);
  }

  void print(RawString str, VGAColor foreground, VGAColor background) {
    print(str, ((uint8)background << 4) + (uint8)foreground);
  }

  void print(RawString str, uint8 color) {
    uint32 col = current_column;
    for(uint16 i=0; str[i]; i++) {
      put(current_row, col, definePixel(str[i], color));
      col += 1;
      if(col >= COLUMNS) {
        col = 0;
        newline();
      }
    }

    current_column = col;
  }

  /*void write(RawString str) {
    uint32 col = current_column;
    for(uint16 i=0; str[i]; i++) {
      put(current_row, col, definePixel(str[i], current_color));
      col += 1;
      if(col >= COLUMNS) {
        col = 0;
        newline();
      }
    }
  }*/

  void puts(RawString str, VGAColor foreground, VGAColor background) {
    print(str, foreground, background);
    newline();
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
