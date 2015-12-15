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

    void endInterrupt() {
      _commandPort.write(Commands::EOI);
    }

    uint8 readCommand() {
      return _commandPort.read();
    }

    void writeCommand(uint8 cmd) {
      _commandPort.write(cmd);
    }

    void writeData(uint8 data) {
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

    void endInterrupt(uint8 irq) {
      if(irq > 8)
        _pics[1].endInterrupt();
      _pics[0].endInterrupt();
    }

    void remap() {
      uint8 a1, a2;

    	a1 = _pics[0].readCommand();
    	a2 = _pics[1].readCommand();

      _pics[0].writeCommand(Pic::Commands::INIT);
      _pics[1].writeCommand(Pic::Commands::INIT);

      _pics[0].writeData(_pics[0].offset());
      _pics[1].writeData(_pics[1].offset());

      _pics[0].writeData(4);
      _pics[1].writeData(2);

      _pics[0].writeData(ICW4_8086);
      _pics[1].writeData(ICW4_8086);

      _pics[0].writeData(a1);
      _pics[1].writeData(a2);
    }
  };
}
