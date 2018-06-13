class VGATerminal {
  static uint16 const ROWS = 25;
  static uint16 const COLUMNS = 80;
  //static uint16* const VGA_MEMORY_ADDRESS = (uint64)0xB8000;

  uint16* const _vga_memory;
  Coloring _current_color;
  uint16 _current_row;
  uint16 _current_column;

public:

  VGATerminal() : _vga_memory((uint16*)0xB8000) {
    _current_color = TermColorings::WhiteOnBlack;
    _current_row = 0;
    _current_column = 0;
  }

  VGATerminal(uint16 row, uint16 column)
    : _vga_memory((uint16*)0xB8000),
      _current_row(row),
      _current_column(column) {
    _current_color = TermColorings::WhiteOnBlack;
  }

  void print(bool value) {
    print(value ? "true" : "false");
  }

  template<class T>
  void print(T arg) {
    print(Helvede::String::to_string(arg));
  }

  template<class T, class... Args>
  void print(T arg, Args... args) {
    print(arg);
    print(args...);
  }

  template<class Size>
  void print(BoundedString<Size> string) {
    uint32 col = _current_column;

    for(Size i=0; i < string.length(); i++) {
      put(_current_row, col, definePixel(string.rawString()[i], _current_color.value));
      col += 1;
      if(col >= COLUMNS) {
        col = 0;
        newline();
      }
    }

    _current_column = col;
  }

  template<class Size>
  void print(BoundedString<Size> string, Coloring coloring) {
    Coloring prev_color = _current_color;
    _current_color = coloring;

    print(string);

    _current_color = prev_color;
  }

  template<class Size>
  void print(BoundedString<Size> string, VGAColor foreground, VGAColor background) {
    Coloring prev_color = _current_color;
    _current_color = Coloring(foreground, background);

    print(string);

    _current_color = prev_color;
  }

  void print(RawString str) {
    print(str, _current_color);
  }

  void print(RawString str, VGAColor foreground, VGAColor background) {
    print(str, Coloring(foreground, background));
  }

  void print(RawString str, Coloring color) {
    uint32 col = _current_column;
    for(uint16 i=0; str[i]; i++) {
      put(_current_row, col, definePixel(str[i], color.value));
      col += 1;
      if(col >= COLUMNS) {
        col = 0;
        newline();
      }
    }

    _current_column = col;
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


  void puts(const char* const str, Coloring coloring) {
    print(str, coloring);
    newline();
  }

  template<class T, class... Args>
  void puts(T arg, Args... args) {
    print(arg);
    print(args...);
    newline();
  }

  template<class T>
  void puts(T arg, Coloring coloring) {
    print(arg, coloring);
    newline();
  }

  template<class T>
  void puts(T arg) {
    print(arg);
    newline();
  }

  // void puts(bool value) {
  //   print(value);
  //   newline();
  // }

  template<class Size>
  void puts(BoundedString<Size> string) {
    print(string);
    newline();
  }

  template<class Size>
  void puts(BoundedString<Size> string, VGAColor foreground, VGAColor background) {
    print(string, foreground, background);
    newline();
  }

  void puts(RawString str, VGAColor foreground, VGAColor background) {
    print(str, foreground, background);
    newline();
  }

  void puts(const char * const str) {
    print(str);
    newline();
  }

  void newline() {
    _current_row += 1;
    _current_column = 0;
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
    _vga_memory[row * COLUMNS + column] = pixel;
  }
};
