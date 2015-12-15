namespace Helvede {
  template<class T>
  class Port {
    /*
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
      uint32 result;
      asm(
        "mov %0, eax\n"
        "mov %1, ebx\n"
        "inb al, bl\n"
        : "=r"(result)
        : "r"(_number)
        :
      );
      return (uint8)result;
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
        ".intel_syntax noprefix\n"
        "mov eax, 4567367\n"
        //"mov %0, eax\n"
        "mov %1, dx\n"
        "ind eax, dx\n"
        : "=r"(result)
        : "r"(_number)
        : "eax", "edx"
      );
      return result;
    }
  */
  };

  template<>
  class Port<uint8> {
    const uint16 _number;

  public:
    Port(uint16 number) : _number(number) {}
    
    void write(uint8 value) {
      asm(
        ".intel_syntax noprefix\n"
        "mov %0, al\n"
        "mov %1, dx\n"
        "outb dx, al\n"
        :
        : "r"(value), "r"(_number)
        : "eax", "edx"
      );
    }


    uint8 read() {
      uint8 result;
      asm(
        ".intel_syntax noprefix\n"
        "mov %0, al\n"
        "mov %1, dx\n"
        "inb al, dx\n"
        : "=r"(result)
        : "r"(_number)
        : "eax", "edx"
      );
      return result;
    }

  };
}
