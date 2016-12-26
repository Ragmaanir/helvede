
extern "C" {
  void keyboard_isr_handler();
  void empty_isr_handler();
  //void common_isr_handler();

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
  void core_isr_21();
  void core_isr_22();
  void core_isr_23();
  void core_isr_24();
  void core_isr_25();
  void core_isr_26();
  void core_isr_27();
  void core_isr_28();
  void core_isr_29();
  void core_isr_30();
  void core_isr_31();
  void core_isr_32();
  void core_isr_33();
}

extern "C" void keyboard_isr_handler_callback() {
  VGATerminal t;
  t.puts("IRQ:YYY");
}

extern "C" void empty_isr_handler_callback() {
  VGATerminal t;
  t.puts("IRQ:empty");
}

extern "C" void common_isr_handler_callback() {
  VGATerminal t;
  t.puts("IRQ:XXX");
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

      t.puts(
        "core_isr_0: ", (uint64)core_isr_0,
        " interrupt_routines[0]: ", (uint64)interrupt_routines[0]
      );

      // for(uint32 i=0; i < sizeof(interrupt_routines)/sizeof(interrupt_routines[0]); i++) {
      //   _entries[_chained_pics.first().offset()+i] = Entry((void*)interrupt_routines[i]);
      // }

      uint8 offset = _chained_pics.first().offset();

      for(uint32 i=0; i < 16; i++) {
        _entries[offset+i] = Entry((void*)interrupt_routines[i]);
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
    typedef void (*ISR_Pointer)();

    ISR_Pointer interrupt_routines[34] = {
      core_isr_0,
      core_isr_1,
      core_isr_2,
      core_isr_3,
      core_isr_4,
      core_isr_5,
      core_isr_6,
      core_isr_7,
      core_isr_8,
      core_isr_9,
      core_isr_10,
      core_isr_11,
      core_isr_12,
      core_isr_13,
      core_isr_14,
      core_isr_15,
      core_isr_16,
      core_isr_17,
      core_isr_18,
      core_isr_19,
      core_isr_20,
      core_isr_21,
      core_isr_22,
      core_isr_23,
      core_isr_24,
      core_isr_25,
      core_isr_26,
      core_isr_27,
      core_isr_28,
      core_isr_29,
      core_isr_30,
      core_isr_31,
      core_isr_32,
      core_isr_33
    };

  private:

    Entry _entries[IDT_ENTRY_COUNT];
    ChainedPics _chained_pics;
    IDTDescriptor _idt_desc;
    VGATerminal& _t;
  };

}
