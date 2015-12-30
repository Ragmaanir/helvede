extern void* irq_keyboard_handler;
extern void* empty_irq_handler;

namespace Helvede {

  extern "C" void keyboard_handler_main() {

  }

  class InterruptDescriptorTable {
  public:

    static const uint32 IDT_ENTRY_COUNT = 256;

    union Entry {
      uint32 data[4];

      // __attribute__(packed)
      // struct Structured {
      //   uint16 offset_low;
      //   uint16 selector;   // a code segment selector in GDT or LDT
      //   uint16 flags;
      //   uint16 offset_mid;
      //   uint32 offset_high;
      //   uint64 zero;
      // } structured;
      //
      // static_assert(sizeof(Structured) == 16, "Structured size");

      Entry() : Entry(0) {}

      Entry(void* ptr) {
        uint64 address = (uint64)ptr;

        uint32 flags = (0x1 << 15) | (0xE << 7);
        const uint32 WORD = 0xffff;
        const uint32 DWORD = 0xffffffff;

        /*
          2 | offset low
          2 | selector
          1 | zero
          1 | type/attributes
          2 | offset middle
          ---
          4 | offset high
          4 | zero
          --------
          8+8 bytes
        */

        /*
          00000000.00001000.10001110.00000000.00101001.00111100.10111000.01001000
          00101000.00111100.10111000.01001000.00000000.00000000.00000000.00000000

          10111000.01001000.00000000.00001000
          00101101.01111000.10000111.00000000

          Bochs check
          00000000.00000000.00011111.00000000.00000000.00000000.00000000.00000000
          IDT
          10111000.01001000.00000000.00001000.00101101.01111000.10000111.00000000
          00000000.00000000.00000000.00000000.00000000.00000000.00000000.00010000
        */

        data[0] =
          ((address & WORD) << 16) |
          (0x8);

        data[1] =
          (address & (WORD << 16)) |
          flags;

        data[2] = (address >> 32) & DWORD;
        data[3] = 0;

        // data[0] =
        //   (address & F4) |
        //   ((uint64)0x8 << 48) |
        //   (0 << 40) |
        //   (flags << 32) |
        //   (address & (F4 << 16));
        //
        // data[1] = (address & F8) << 32;
      }
    };

    static_assert(sizeof(Entry) == 16, "Entry size");

    class IDTDescriptor {
    public:
      uint64 data[2];

      IDTDescriptor() : data({0,0}) {}

      IDTDescriptor(Entry *base, uint16 limit) {
        uint64 address = (uint64)base;
        // upper = ((uint64)limit << 48) + (address >> 16);
        // lower = (address & 0xffff) <<  48;
        data[0] = limit + ((address & 0xffffffffffff) << 16);
        data[1] = address & (0xffff << 48);
      }
    };

    static_assert(sizeof(IDTDescriptor) == 16, "IDTDescriptor size");

    InterruptDescriptorTable(VGATerminal& t) : _chained_pics(0x20, 0x28), _t(t) {

    }

    void install() {
      //uint64 idt_addr = ((uint64)&_entries);
      //_idt_desc.base = ((uint64)&_entries);
      //_idt_desc.limit = IDT_ENTRY_COUNT * sizeof(Entry);
      //_idt_desc.limit = 64 * 10;

      //_idt_desc.limit = (IDT_ENTRY_COUNT * sizeof(Entry)) + ((idt_addr & 0xffff) << 16);
      //_idt_desc.base = idt_addr >> 16;
      // _idt_desc.limit = idt_addr & 0xffff0000;
      // _idt_desc.base = idt_addr & 0xffff;

      _idt_desc = IDTDescriptor(_entries, 0x00ff);

      _t.puts("---- ENTRIES POINTER");
      _t.puts(String::to_string((uint64)_entries, 2));
      _t.puts("---- UPPER & LOWER");
      _t.puts(String::to_string(_idt_desc.data[0], 2));
      _t.puts(String::to_string(_idt_desc.data[1], 2));

      if((uint64)_entries > ((_idt_desc.data[0] & 0xffffffffffff) << 16) + (_idt_desc.data[1] >> 48)) {
        _t.puts("ENTRYPTR > ADDR/LIMIT");
        _t.puts(String::to_string((_idt_desc.data[0] & 0xffffffffffff) << 16, 2));
        _t.puts(String::to_string(_idt_desc.data[1] >> 48, 2));
      }

      asm(
        "mov rax, %0\n"
        "lidt [rax]\n"
        :: "r"(&_idt_desc)
      );

      _chained_pics.remap();

      Entry entry = Entry(irq_keyboard_handler);
      _entries[_chained_pics.first().offset()] = entry;

      /*
        00000000.00001000.10001110.00000000.00101001.00111100.10111000.01001000
        00101000.00111100.10111000.01001000.00000000.00000000.00000000.00000000
      */

      asm("sti" ::);
    }

  private:

    Entry _entries[IDT_ENTRY_COUNT];
    ChainedPics _chained_pics;
    IDTDescriptor _idt_desc;
    VGATerminal& _t;
  };
}
