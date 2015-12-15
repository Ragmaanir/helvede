#define $assertEqual(l,r) assertEqual(l,r, (#l " == " #r))

namespace Helvede {
  class Tests {

    VGATerminal terminal;

    void assertEqual(uint32 left, uint32 right, RawString msg) {
      if(left != right) {
        terminal.print("FAIL ", VGATerminal::VGAColor::Red, VGATerminal::VGAColor::Black);
      } else {
        terminal.print("OK   ", VGATerminal::VGAColor::Green, VGATerminal::VGAColor::Black);
      }

      terminal.puts(msg, VGATerminal::VGAColor::Gray, VGATerminal::VGAColor::Black);
    }

  public:
    Tests(VGATerminal t) : terminal(t) {}

    void testStaticArray() {
      StaticArray<uint32, 32> arr;

      arr.set(0, 1337);

      terminal.puts(Uint32(arr[0]).toString());
      terminal.puts(Uint32(arr.size()).toString());

      $assertEqual(arr[0], 1337);
      $assertEqual(arr.size(), 32);

      //arr.each([&](uint16 v) -> void { t.puts(Uint16(v).toString()); });
      //arr.each([](uint16 v) -> void { 2; });
    }

    void testPorts() {
      Port<uint8> p(0x60);

      uint8 c = 0;

      /*while(c == 0) */c = p.read();

      terminal.print("IN: ");
      terminal.puts(Uint8(c).toString());
    }

    void run() {
      terminal.puts("BEGIN Tests ----------");
      testStaticArray();
      testPorts();
      terminal.puts("END Tests ----------");
    }
  };
}
