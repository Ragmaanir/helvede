namespace Helvede {
  template<class T>
  class Port {

  };

  template<>
  class Port<uint8> {
    const uint16 _number;

  public:
    Port(uint16 number) : _number(number) {}

    void write(uint8 value) {
      asm volatile(
        "mov al, %0\n"
        "mov dx, %1\n"
        "outb dx, al\n"
        :
        : "r"(value), "r"(_number)
        : "eax", "edx"
      );
    }

    uint8 read() {
      uint8 result;
      asm volatile(
        "mov al, %0\n"
        "mov dx, %1\n"
        "inb al, dx\n"
        : "=r"(result)
        : "r"(_number)
        : "eax", "edx"
      );
      return result;
    }
  };
}
