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

    void end_interrupt()          { _commandPort.write(Commands::EOI); }

    uint8 read_command()          { return _commandPort.read(); }
    void write_command(uint8 cmd) { _commandPort.write(cmd); }

    uint8 read_data()             { return _dataPort.read(); }
    void write_data(uint8 data)   { _dataPort.write(data); }

    uint8 offset() const          { return _offset; }
  };
}
