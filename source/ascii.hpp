namespace Helvede {
  namespace Ascii {
    uint8 decimal_to_code(uint8 n) {
      // FIXME raise if n > 9
      return 48 + n;
    }
  }
}
