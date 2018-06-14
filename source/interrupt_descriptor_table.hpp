
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
  void empty_isr_handler();

  void core_isr_0();
  void core_isr_1();
  void core_isr_2();
  void core_isr_3();
  void core_isr_4();
  void core_isr_5();
  void core_isr_6();
  void core_isr_7();
  void core_isr_8();
  void core_isr_9();
  void core_isr_10();
  void core_isr_11();
  void core_isr_12();
  void core_isr_13();
  void core_isr_14();
  void core_isr_15();
  void core_isr_16();
  void core_isr_17();
  void core_isr_18();
  void core_isr_19();
  void core_isr_20();
  void core_isr_64();

}

extern "C" void empty_isr_handler_callback() {
  VGATerminal t(8, 30);
  t.puts("IRQ:empty");
}

namespace Helvede {

  /*
  0x0000     | 0x00      | Divide by 0
  0x0004     | 0x01      | Reserved
  0x0008     | 0x02      | NMI Interrupt
  0x000C     | 0x03      | Breakpoint (INT3)
  0x0010     | 0x04      | Overflow (INTO)
  0x0014     | 0x05      | Bounds range exceeded (BOUND)
  0x0018     | 0x06      | Invalid opcode (UD2)
  0x001C     | 0x07      | Device not available (WAIT/FWAIT)
  0x0020     | 0x08      | Double fault
  0x0024     | 0x09      | Coprocessor segment overrun
  0x0028     | 0x0A      | Invalid TSS
  0x002C     | 0x0B      | Segment not present
  0x0030     | 0x0C      | Stack-segment fault
  0x0034     | 0x0D      | General protection fault
  0x0038     | 0x0E      | Page fault
  0x003C     | 0x0F      | Reserved
  0x0040     | 0x10      | x87 FPU error
  0x0044     | 0x11      | Alignment check
  0x0048     | 0x12      | Machine check
  0x004C     | 0x13      | SIMD Floating-Point Exception
  0x00xx     | 0x14-0x1F | Reserved
  0x0xxx     | 0x20-0xFF | User definable
  */

  enum class Interrupt {
    DivideByZero = 0,
    NMIInterrupt = 2
  };

  // char* lower_interrupt_names[14] = {
  //   {"Divide by zero"},
  //   {"Reserved"},
  //   {"NMI Interrupt"},
  //   {"Breakpoint"},
  //   {"Overflow"},
  //   {"Bounds range exceeded"},
  //   {"Invalid opcode"},
  //   {"Device not available (WAIT/FWAIT)"},
  //   {"Double fault"},
  //   {"Coprocessor segment overrun"},
  //   {"Invalid TSS"},
  //   {"Segment not present"},
  //   {"Stack-segment fault"},
  //   {"General protection fault"},
  //   {"Page fault"},
  //   {"Reserved"},
  //   {"x87 FPU error"},
  //   {"Alignment check"},
  //   {"Machine check"},
  //   {"SIMD Floating-Point Exception"}
  // };

  const char* lower_interrupt_names(uint32 i) {
    switch(i) {
      case 0: return "Divide by zero";
      case 1: return "Reserved";
      case 2: return "NMI Interrupt";
      case 3: return "Breakpoint";
      case 4: return "Overflow";
      case 5: return "Bounds range exceeded";
      case 6: return "Invalid opcode";
      case 7: return "Device not available (WAIT/FWAIT)";
      case 8: return "Double fault";
      case 9: return "Coprocessor segment overrun";
      case 10: return "Invalid TSS";
      case 11: return "Segment not present";
      case 12: return "Stack-segment fault";
      case 13: return "General protection fault";
      case 14: return "Page fault";
      case 15: return "Reserved";
      case 16: return "x87 FPU error";
      case 17: return "Alignment check";
      case 18: return "Machine check";
      case 19: return "SIMD Floating-Point Exception";
      default: return "-- error --";
    }
  };

  const char* interrupt_name(uint32 i) {
    if(i < 0x14) {
      return lower_interrupt_names(i);
    } else if (i < 0x20) {
      return "Reserved";
    } else {
      return "User definable";
    }
  }

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
      //return (uint64)core_isr_1 - (uint64)core_isr_0;
      // FIXME: strangely core_isr_0/1 etc return the same values which makes the size zero ...
      return 64;
    }

    void install() {
      VGATerminal t = _t;

      _chained_pics.remap();

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

      uint8 offset = _chained_pics.first().offset();
      uint64 size = isr_size();

      $assert(size > 0 && size < 64);
      $assert((uint64)core_isr_0 + size*64 == (uint64)core_isr_64);

      {
        VGATerminal t(10, 0);

        t.puts(String::to_string((uint64)core_isr_0, 16), TermColorings::WhiteOnRed);
        t.puts(String::to_string((uint64)core_isr_1, 16), TermColorings::WhiteOnRed);
        t.puts(String::to_string((uint64)empty_isr_handler, 16), TermColorings::WhiteOnRed);
      }

      /*
      for(uint32 i=0; i < 256; i++) {
        _entries[offset+i] = Entry((void*)((uint64)core_isr_0 + size*i));
      }
      */

      _entries[offset+0] = Entry((void*)((uint64)core_isr_0));
      _entries[offset+1] = Entry((void*)((uint64)core_isr_1));
      _entries[offset+2] = Entry((void*)((uint64)core_isr_2));
      _entries[offset+3] = Entry((void*)((uint64)core_isr_3));
      _entries[offset+4] = Entry((void*)((uint64)core_isr_4));
      _entries[offset+5] = Entry((void*)((uint64)core_isr_5));
      _entries[offset+6] = Entry((void*)((uint64)core_isr_6));
      _entries[offset+7] = Entry((void*)((uint64)core_isr_7));
      _entries[offset+8] = Entry((void*)((uint64)core_isr_8));
      _entries[offset+9] = Entry((void*)((uint64)core_isr_9));
      _entries[offset+10] = Entry((void*)((uint64)core_isr_10));
      _entries[offset+11] = Entry((void*)((uint64)core_isr_11));

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

// extern "C" __attribute__((noinline)) void non_inline() {
//   ((uint16*)0xB8000)[10 + 10 * 80] = 0x2f4b2f4f;
// }

static uint64 invocation_count = 0;

extern "C" void common_isr_handler_callback(uint64 i) {
  invocation_count += 1;
  // if(i == 1) {
  //   VGATerminal t(18,20);
  //   t.puts("KEYBOARD");
  // } else {
  //   VGATerminal t(16,20);
  //   t.puts("IRQ #", i, "(", Helvede::interrupt_name(i), ")" " count: ", invocation_count);
  //   //t.puts("IRQ #", i, "(", Helvede::interrupt_name(i), ")" " count: ", Helvede::String::to_string(invocation_count));
  //   //t.puts("IRQ #", i, "(", Helvede::interrupt_name(i), ")");
  //   // t.puts("1234");
  // }

  // Helvede::Dbg::put(i, 22, Helvede::Ascii::decimal_to_code((uint8)i), 0x3f);

  VGATerminal t(16,20);
  t.puts("IRQ #", i, "(", Helvede::interrupt_name(i), "), count: ", Helvede::String::to_string(invocation_count));
}
