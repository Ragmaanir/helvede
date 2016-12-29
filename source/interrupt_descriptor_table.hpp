
#define $$concat(a, ...) $$concat_f(a, __VA_ARGS__)
#define $$concat_f(a, ...) a ## __VA_ARGS__
// #define $$empty(...)
// #define $repeat_f() $repeat
// #define $repeat(n, exp) exp && $$eval($repeat_f)()(n-1, exp)
// #define $$eval(...) __VA_ARGS__ $$empty()
// #define $$eval_f(f, args) (f ## args) $$empty()

#define $repeat(n, exp) $repeat_##n(exp)
#define $repeat_1(exp) exp;
#define $repeat_2(exp) exp; $repeat_1(exp)
#define $repeat_3(exp) exp; $repeat_2(exp)
#define $repeat_4(exp) exp; $repeat_3(exp)
#define $repeat_5(exp) exp; $repeat_4(exp)
#define $repeat_6(exp) exp; $repeat_5(exp)
#define $repeat_7(exp) exp; $repeat_6(exp)
#define $repeat_8(exp) exp; $repeat_7(exp)
#define $repeat_9(exp) exp; $repeat_8(exp)
#define $repeat_10(exp) exp; $repeat_9(exp)
#define $repeat_11(exp) exp; $repeat_10(exp)

//#define $rep(n, exp) (n==1) ? exp : $rep_r(n, exp)
//#define $rep_r(n, exp) $rep(n-1, exp)
//#define $$delay(name, ...) (($ ## name))(__VA_ARGS__)

// void macros() {
//   int i = 0;
//   $repeat(4, (i += 1));
// }

extern "C" {
  void keyboard_isr_handler();
  void empty_isr_handler();
  //void common_isr_handler();

  void core_isr_0();
  void core_isr_1();
  void core_isr_64();
}

extern "C" void keyboard_isr_handler_callback() {
  VGATerminal t;
  t.puts("IRQ:YYY");
}

extern "C" void empty_isr_handler_callback() {
  VGATerminal t;
  t.puts("IRQ:empty");
}

static uint64 invocation_count = 0;

extern "C" void common_isr_handler_callback(uint64 i) {
  invocation_count += 1;
  if(i == 1) {
    VGATerminal t(18,0);
    t.puts("KEYBOARD");
  }
  else {
    VGATerminal t(16,0);
    t.puts("IRQ #", i, " count: ", invocation_count);
  }
}

namespace Helvede {

  class InterruptDescriptorTable {
  public:

    static const uint32 IDT_ENTRY_COUNT = 256;

    union Entry {
      uint8 data[16];
      struct Structured {
        uint16 offset_low;
        uint16 selector;   // a code segment selector in GDT or LDT
        uint16 flags;
        uint16 offset_mid;
        uint32 offset_high;
        uint32 zero;
      } __attribute__((packed)) structured;

      static_assert(sizeof(Structured) == 16, "Structured size");

      Entry() : Entry((void*)empty_isr_handler) {}

      Entry(void* ptr) {
        uint64 address = (uint64)ptr;

        const uint32 WORD = 0xffff;
        const uint32 DWORD = 0xffffffff;

        structured.offset_low = address & WORD;
        structured.selector = 0x8;
        structured.flags = (0x1 << 15) | (0xE << 8);
        structured.offset_mid = (address >> 16) & WORD;
        structured.offset_high = (address >> 32) & DWORD;
        structured.zero = 0;
      }
    };

    static_assert(sizeof(Entry) == 16, "Entry size");

    union IDTDescriptor {
      uint8 data[16];

      struct Structured {
        uint16 limit;
        uint64 base;
      } __attribute__((packed)) structured;

      IDTDescriptor() {
        IDTDescriptor(0,0);
      }

      IDTDescriptor(Entry *base, uint16 limit) {
        uint64 address = (uint64)base;

        structured.limit = limit;
        structured.base = address;
      }
    };

    static_assert(sizeof(IDTDescriptor) == 16, "IDTDescriptor size");

    InterruptDescriptorTable(VGATerminal& t) : _chained_pics(0x20, 0x28), _t(t) {
      t.puts(
        "PIC: ",
        "0: ", _chained_pics.first().offset(),
        " 1: ", _chained_pics.second().offset()
      );
    }

    uint64 isr_size() {
      return (uint64)core_isr_1 - (uint64)core_isr_0;
    }

    void install() {
      //VGATerminal t(10, 0);
      VGATerminal t = _t;

      t.puts(" ");
      t.puts("First: ", _chained_pics.first().offset());
      t.puts("Second: ", _chained_pics.first().offset());

      _chained_pics.remap();

      t.puts("After Remap: ", _chained_pics.first().offset());
      t.puts("After Remap 2: ", _chained_pics.first().offset());

      t.puts(
        "PIC offset X: ",
        _chained_pics.first().offset(),
        " ",
        _chained_pics.second().offset()
      );

      t.puts(
        "PIC offset Y: ",
        _chained_pics.first().offset(),
        " ",
        _chained_pics.second().offset()
      );

      // for(uint32 i=0; i < sizeof(interrupt_routines)/sizeof(interrupt_routines[0]); i++) {
      //   _entries[_chained_pics.first().offset()+i] = Entry((void*)interrupt_routines[i]);
      // }

      // const uint16 PORT = 0x3f8;
      // Port<uint8>(PORT + 1).write(0x00);
      // Port<uint8>(PORT + 3).write(0x80);
      // Port<uint8>(PORT + 0).write(0x03);
      // Port<uint8>(PORT + 1).write(0x00);
      // Port<uint8>(PORT + 3).write(0x03);
      // Port<uint8>(PORT + 2).write(0xC7);
      // Port<uint8>(PORT + 4).write(0x0B);

      // while (Port<uint8>(PORT + 5).read() & 0x20 == 0);

      // Port<uint8>(PORT).write(0x45);
      // Port<uint8>(PORT).write(0x34);
      // Port<uint8>(PORT).write(0x88);

      // for(int i=0; i< 10000; i++) {
      //   Port<uint8>(PORT).write(i);
      // }

      // outb(PORT + 1, 0x00);    // Disable all interrupts
      // outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
      // outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
      // outb(PORT + 1, 0x00);    //                  (hi byte)
      // outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
      // outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
      // outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set

      uint8 offset = 0; //_chained_pics.first().offset();
      uint64 size = isr_size();

      $assert(isr_size() > 0 && isr_size() < 64);
      $assert((uint64)core_isr_0 + size*64 == (uint64)core_isr_64);

      // for(uint32 i=0; i < 16; i++) {
      //   _entries[offset+i] = Entry((void*)interrupt_routines[i]);
      // }

      for(uint32 i=0; i < 64; i++) {
        //_entries[offset+i] = Entry((void*)interrupt_routines[i]);
        _entries[offset+i] = Entry((void*)((uint64)core_isr_0 + size*i));
      }

      _idt_desc = IDTDescriptor(_entries, IDT_ENTRY_COUNT * sizeof(Entry) - 1);

      asm(
        "mov rax, %0\n"
        "lidt [rax]\n"
        :: "r"(&_idt_desc)
      );

      asm("sti" ::);
      asm("xchg bx, bx" ::);
      _chained_pics.first().write_data(0xFD);
      _chained_pics.first().write_data(0);
    }

    /*
    * Raw pointer table to ISRs defined in assembler.
    */
    /*
    typedef void (*ISR_Pointer)();

    ISR_Pointer interrupt_routines[46] = {
      core_isr_0,
      core_isr_1,
      core_isr_2
    };
    */

  private:

    Entry _entries[IDT_ENTRY_COUNT];
    ChainedPics _chained_pics;
    IDTDescriptor _idt_desc;
    VGATerminal& _t;
  };

}
