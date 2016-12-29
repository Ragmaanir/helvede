
typedef uint8 color;

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

struct Coloring {
  uint8 value;

  uint8 foreground() const { return value & 0xf; };
  uint8 background() const { return (value >> 4) & 0xf; };

  // Coloring(VGAColor foreground, VGAColor background)
  // : value(((uint8)background << 4) + (uint8)foreground) {}

  constexpr Coloring(VGAColor foreground, VGAColor background)
    : value(((uint8)background << 4) + (uint8)foreground) {}

  constexpr explicit Coloring(uint8 c) : value(c) {}

  constexpr Coloring() : Coloring(VGAColor::LightBlue, VGAColor::Black) {}
} __attribute__((packed));

namespace TermColorings {
  static Coloring WhiteOnBlack;
  static Coloring BlackOnWhite;
  static Coloring GreenOnBlack;
  static Coloring RedOnBlack;
  static Coloring WhiteOnRed;

  constexpr void static_init() {
    WhiteOnBlack = Coloring(VGAColor::White, VGAColor::Black);
    BlackOnWhite = Coloring(VGAColor::Black, VGAColor::White);
    GreenOnBlack = Coloring(VGAColor::Green, VGAColor::Black);
    RedOnBlack = Coloring(VGAColor::Red, VGAColor::Black);
    WhiteOnRed = Coloring(VGAColor::White, VGAColor::Red);
  }
};

namespace Helvede {
  namespace String {
    template<>
    Letters to_string(Coloring value, uint8 base) {
      return to_string(value.value, base);
    }
  }
}
