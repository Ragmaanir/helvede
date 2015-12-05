namespace Helvede {
  class Port {
    const uint16 _number;
  public:
    Port(uint16 number) : _number(number) {}

    void write(uint8 value) {
      asm(
        "mov %0, %%ax"
        "mov %1, %%al"
        "outb %%ax, %%al;"
        :
        : "r"(value), "r"(_number)
      );
    }

    void write(uint16 value) {
      asm(
        "mov %0, %%ax"
        "mov %1, %%al"
        "outw %%ax, %%al;"
        :
        : "r"(value), "r"(_number)
      );
    }

    void write(uint32 value) {
      asm(
        "mov %0, %%eax"
        "mov %1, %%al"
        "outl %%eax, %%al;"
        :
        : "r"(value), "r"(_number)
      );
    }

    uint8 read8() {
      uint8 result;
      asm(
        "inb %1"
        : "=r"(result)
        : "r"(_number)
      );
      return result;
    }

    uint16 read16() {
      uint16 result;
      asm(
        "inw %1"
        : "=r"(result)
        : "r"(_number)
      );
      return result;
    }

    uint32 read32() {
      uint32 result;
      asm(
        "inl %1"
        : "=r"(result)
        : "r"(_number)
      );
      return result;
    }
  };
}
