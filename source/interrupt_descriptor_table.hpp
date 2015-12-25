extern void* irq_keyboard_handler;
extern void* empty_irq_handler;

namespace Helvede {

  extern "C" void keyboard_handler_main() {

  }

  class InterruptDescriptorTable {
  public:

    static const uint32 IDT_ENTRY_COUNT = 256;

    union Entry {
      uint64 data[2];

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

      Entry() : Entry(0) {
        /*offset_low = 0;
        offset_high = 0;
        zero = 0;
        selector = 0x8;
        type_attr = 0x8e;*/
      }

      Entry(void* ptr) {
        // offset_low = address & 0xffff;
        // offset_high = address & 0xffff0000 >> 16;
        // zero = 0;
        // selector = 0x8; // TODO: KERNEL CODE SEGMENT
        // type_attr = 0x8e;
        uint64 address = (uint64)ptr;

        // offset_low = address >> 48;
        // offset_mid = (address >> 32) & 0xffff;
        // offset_high = address & 0xffff;
        // selector = 0x8;
        // flags = 0;
        // zero = 0;

        uint64 flags = (0x1 << 15) | (0xE << 8);
        const uint64 F4 = 0xffff;
        const uint64 F8 = 0xffffffff;

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

        data[0] =
          (address & F4) |
          ((uint64)0x8 << 48) |
          (0 << 40) |
          (flags << 32) |
          (address & (F4 << 16));

        data[1] = (address & F8) << 32;
      }
    };

    static_assert(sizeof(Entry) == 16, "Entry size");

    // struct Entry {
    //   uint16  offset_low;
    //   uint16  selector;   // a code segment selector in GDT or LDT
    //   uint8   zero;       // unused, set to 0
    //   uint8   type_attr;  // type and attributes
    //   uint16  offset_high;
    //
    //   Entry() : Entry(0) {
    //     /*offset_low = 0;
    //     offset_high = 0;
    //     zero = 0;
    //     selector = 0x8;
    //     type_attr = 0x8e;*/
    //   }
    //
    //   Entry(uint32 address) {
    //     offset_low = address & 0xffff;
    //     offset_high = address & 0xffff0000 >> 16;
    //     zero = 0;
    //     selector = 0x8; // TODO: KERNEL CODE SEGMENT
    //     type_attr = 0x8e;
    //   }
    // };

    // class IDTDescriptor {
    //   const uint32 _data;
    //
    //   IDTDescriptor(uint32 base, uint16 limit) : _data() {}
    //
    //   static uint32 build_descriptor(uint32 base, uint16 limit) {
    //
    //   }
    // };

    class IDTDescriptor {
    public:
      uint64 upper;
      uint64 lower;

      IDTDescriptor() : upper(0), lower(0) {}

      IDTDescriptor(Entry *base, uint16 limit) {
        uint64 address = (uint64)base;
        upper = limit + (address >> 16);
        lower = (address & 0xffff) <<  24;
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

      _idt_desc = IDTDescriptor(_entries, 64*10);

      asm(
        "mov rax, %0\n"
        "lidt [rax]\n"
        :: "r"(&_idt_desc)
      );

      _chained_pics.remap();

      _entries[_chained_pics.first().offset()] = Entry(irq_keyboard_handler);

      _t.puts(String::to_string((uint64)irq_keyboard_handler, 2));
      _t.puts("1000100011100000000000101001001111001011100001001000");
      _t.puts(String::to_string(Entry(irq_keyboard_handler).data[0], 16));
      _t.puts("10100000111100101110000100100000000000000000000000000000000000");
      _t.puts(String::to_string(Entry(irq_keyboard_handler).data[1], 16));

      asm("sti" ::);
    }

  private:

    Entry _entries[IDT_ENTRY_COUNT];
    ChainedPics _chained_pics;
    IDTDescriptor _idt_desc;
    VGATerminal& _t;
  };
}
