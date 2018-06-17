namespace Helvede {
  // Can be used as GDT and IDT descriptor
  union TableDescriptor {
    uint8 data[6];

    struct Structured {
      uint16 limit;
      uint32 base;
    } __attribute__((packed)) structured;

    TableDescriptor() {
      TableDescriptor(0,0);
    }

    TableDescriptor(void *base, uint32 limit) {
      // FIXME: raise if limit > 0xffff
      uint32 address = (uint32)((uint64)base);

      structured.limit = limit - 1;
      structured.base = address;
    }
  };

  static_assert(sizeof(TableDescriptor) == 6, "TableDescriptor size");
  static_assert(sizeof(TableDescriptor::Structured) == 6, "TableDescriptor structure size");
}
