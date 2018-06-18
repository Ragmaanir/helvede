namespace Helvede {
  class ChainedPics {
    Pic _pics[2];

  public:

    enum SetupData { ICW4_8086 = 0x1 };

    ChainedPics(uint8 offset1, uint8 offset2) :
      _pics({
        Pic(offset1, Port<uint8>(0x20), Port<uint8>(0x21)),
        Pic(offset2, Port<uint8>(0xA0), Port<uint8>(0xA1))
      })
    {}

    Pic& first()  { return _pics[0]; }
    Pic& second() { return _pics[1]; }

    void end_interrupt(uint8 irq) {
      if(irq > 8)
        _pics[1].end_interrupt();
      _pics[0].end_interrupt();
    }

    void wait() {
      Port<uint8> delay_port = Port<uint8>(0x80);

      delay_port.write(Pic::Commands::INIT);
    }

    // void remap() {
    //   asm volatile(
    //     "movb al, 0x11\n"
    //     "outb 0x20, al\n"
    //     "outb 0xa0, al\n"

    //     "movb al, 0x20\n"
    //     "outb 0x21, al\n"
    //     "movb al, 0x28\n"
    //     "outb 0xa1, al\n"

    //     "movb al, 0x4\n"
    //     "outb 0x21, al\n"
    //     "movb al, 0x2\n"
    //     "outb 0xa1, al\n"

    //     "movb al, 0x1\n"
    //     "outb 0x21, al\n"
    //     "movb al, 0x1\n"
    //     "outb 0xa1, al\n"

    //     "movb al, 0xfd\n"
    //     "outb 0x21, al\n"
    //     "movb al, 0xff\n"
    //     "outb 0xa1, al\n"
    //     ::
    //   );
    // }

    void remap() {
      uint8 mask1, mask2;

      mask1 = _pics[0].read_data();
      mask2 = _pics[1].read_data();

      _pics[0].write_command(Pic::Commands::INIT);
      // wait();

      _pics[1].write_command(Pic::Commands::INIT);
      // wait();

      _pics[0].write_data(_pics[0].offset());
      // wait();

      _pics[1].write_data(_pics[1].offset());
      // wait();

      _pics[0].write_data(4);
      // wait();

      _pics[1].write_data(2);
      // wait();

      _pics[0].write_data(ICW4_8086);
      // wait();

      _pics[1].write_data(ICW4_8086);
      // wait();

      // _pics[0].write_data(mask1);
      // _pics[1].write_data(mask2);

      // /* mask to allow only keyboard */
      _pics[0].write_data(0xfd);
      _pics[1].write_data(0xff);
    }
  };
}
