#include "table_descriptor.hpp"

extern "C" {
  void empty_isr_handler();
}

// Defined in assembly file
extern "C" uint64 isr_pointer_table[];

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

  static const char* const lower_interrupt_name_table[] = {
    "Divide by zero",
    "Reserved",
    "NMI Interrupt",
    "Breakpoint",
    "Overflow",
    "Bounds range exceeded",
    "Invalid opcode",
    "Device not available (WAIT/FWAIT)",
    "Double fault",
    "Coprocessor segment overrun",
    "Invalid TSS",
    "Segment not present",
    "Stack-segment fault",
    "General protection fault",
    "Page fault",
    "Reserved",
    "x87 FPU error",
    "Alignment check",
    "Machine check",
    "SIMD Floating-Point Exception"
  };

  const char* const lower_interrupt_names(uint32 i) {
    if(i < 20) {
      return lower_interrupt_name_table[i];
    } else {
      return "No name for interrupt defined";
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

    InterruptDescriptorTable(VGATerminal& t) : _chained_pics(0x20, 0x28), _t(t) {
      t.puts(
        "PIC: ",
        "0: ", _chained_pics.first().offset(),
        " 1: ", _chained_pics.second().offset()
      );
    }

    void install() {
      VGATerminal t = _t;

      _chained_pics.remap();

      //uint8 offset = _chained_pics.first().offset();

      for(uint32 i = 0; i < 256; i++) {
        _entries[i] = Entry((void*)isr_pointer_table[i]);
      }


      _idt_desc = TableDescriptor(_entries, IDT_ENTRY_COUNT * sizeof(Entry));

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

  private:

    Entry _entries[IDT_ENTRY_COUNT];
    ChainedPics _chained_pics;
    TableDescriptor _idt_desc;
    VGATerminal& _t;
  };

}


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

  Helvede::Dbg::put(i, 17, Helvede::Ascii::decimal_to_code((uint8)i), 0x3f);

  VGATerminal t(16,20);
  t.puts("IRQ #", i, "(", Helvede::interrupt_name(i), "), count: ", Helvede::String::to_string(invocation_count));
}
