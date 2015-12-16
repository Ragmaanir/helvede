#define $assert_equal(l,r) assert_equal(l,r, (#l " == " #r))

namespace Helvede {
  class Tests {

    VGATerminal terminal;

    template<class A, class B>
    void assert_equal(A left, B right, RawString msg) {
      if(left == right) {
        terminal.print("OK   ", VGATerminal::VGAColor::Green, VGATerminal::VGAColor::Black);
      } else {
        terminal.print("FAIL ", VGATerminal::VGAColor::Red, VGATerminal::VGAColor::Black);
      }

      terminal.puts(msg, VGATerminal::VGAColor::Gray, VGATerminal::VGAColor::Black);
    }

  public:
    Tests(VGATerminal t) : terminal(t) {}

    void test_static_array() {
      StaticArray<uint32, 32> arr;

      arr.set(0, 1337);

      terminal.puts(String::format(arr[0]));
      terminal.puts(String::format(arr.size()));

      $assert_equal(arr[0], 1337);
      $assert_equal(arr.size(), 32);

      //arr.each([&](uint16 v) -> void { t.puts(Uint16(v).toString()); });
      //arr.each([](uint16 v) -> void { 2; });
    }

    void test_number_to_string() {
      //$assert_equal(Uint32(10101).toString(), BoundedString<uint8>("10101"));
    }

    void test_ports() {
      Port<uint8> p(0x60);

      uint8 c = 0;

      /*while(c == 0) */c = p.read();

      terminal.print("IN: ");
      terminal.puts(String::format(c));
    }

    void run() {
      terminal.puts("BEGIN Tests ----------");
      test_static_array();
      test_number_to_string();
      test_ports();
      terminal.puts("END Tests ----------");
    }
  };
}
