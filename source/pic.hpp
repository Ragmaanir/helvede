namespace Helvede {
  class Pic {
    const uint8 _offset;
    Port<uint8> _commandPort;
    Port<uint8> _dataPort;

  public:

    enum Commands : uint8 {
      INIT = 0x11,
      EOI = 0x20
    };

    Pic(uint8 offset, Port<uint8> cmd, Port<uint8> data)
      : _offset(offset), _commandPort(cmd), _dataPort(data) {}

    void end_interrupt() {
      _commandPort.write(Commands::EOI);
    }

    uint8 read_command() {
      return _commandPort.read();
    }

    void write_command(uint8 cmd) {
      _commandPort.write(cmd);
    }

    void write_data(uint8 data) {
      _dataPort.write(data);
    }

    uint8 offset() const {
      return _offset;
    }
  };

  class ChainedPics {
    Pic _pics[2];

  public:

    enum SetupData {
      ICW4_8086 = 0x1
    };

    ChainedPics(uint8 offset1, uint8 offset2) :
      _pics({
        Pic(offset1, Port<uint8>(0x20), Port<uint8>(0x21)),
        Pic(offset2, Port<uint8>(0xA0), Port<uint8>(0xA1))
      })
    {}

    Pic& first() {
      return _pics[0];
    }

    Pic& second() {
      return _pics[1];
    }

    void end_interrupt(uint8 irq) {
      if(irq > 8)
        _pics[1].end_interrupt();
      _pics[0].end_interrupt();
    }

    void remap() {
      uint8 a1, a2;

    	a1 = _pics[0].read_command();
    	a2 = _pics[1].read_command();

      _pics[0].write_command(Pic::Commands::INIT);
      _pics[1].write_command(Pic::Commands::INIT);

      _pics[0].write_data(_pics[0].offset());
      _pics[1].write_data(_pics[1].offset());

      _pics[0].write_data(4);
      _pics[1].write_data(2);

      _pics[0].write_data(ICW4_8086);
      _pics[1].write_data(ICW4_8086);

      _pics[0].write_data(a1);
      _pics[1].write_data(a2);
    }
  };
}
