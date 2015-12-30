#define $assert_equal(l,r) assert_equal(l,r, (#l " == " #r))

namespace Helvede {
  class Tests {

    VGATerminal& terminal;

    template<class A, class B>
    void assert_equal(A left, B right, RawString msg) {
      if(left == right) {
        //terminal.print("OK   ", VGATerminal::VGAColor::Green, VGATerminal::VGAColor::Black);
        terminal.print(".", VGATerminal::VGAColor::Green, VGATerminal::VGAColor::Black);
      } else {
        terminal.newline();
        terminal.print("FAIL ", VGATerminal::VGAColor::Red, VGATerminal::VGAColor::Black);
        terminal.puts(msg, VGATerminal::VGAColor::Gray, VGATerminal::VGAColor::Black);
      }
    }

  public:
    Tests(VGATerminal& t) : terminal(t) {}

    void test_static_array() {
      StaticArray<uint32, 32> arr;

      arr.set(0, 1337);

      $assert_equal(arr[0], 1337);
      $assert_equal(arr.size(), 32);

      //arr.each([&](uint16 v) -> void { t.puts(Uint16(v).toString()); });
      //arr.each([](uint16 v) -> void { 2; });
    }

    void test_string_compare() {
      {
        Letters a = Letters("abcde");
        Letters b = Letters("abcde");

        $assert_equal(a, b);
      }

      {
        Letters a = Letters("abcde");
        Letters b = Letters("accde");

        $assert_equal(a.compare(b), ComparisonResult::LessThan);
        $assert_equal(b.compare(a), ComparisonResult::GreaterThan);
      }

      {
        Letters a = Letters("abcde");
        Letters b = Letters("abcdea");

        $assert_equal(a.compare(b), ComparisonResult::LessThan);
        $assert_equal(b.compare(a), ComparisonResult::GreaterThan);
      }

      {
        Letters a = Letters("");
        Letters b = Letters("a");

        $assert_equal(a.compare(b), ComparisonResult::LessThan);
        $assert_equal(b.compare(a), ComparisonResult::GreaterThan);
      }
    }

    void test_number_to_string() {
      $assert_equal(String::to_string(1337, 2).length(), 11);
      $assert_equal(String::to_string(1337, 2), Letters("10100111001"));
      $assert_equal(String::to_string(0, 2), Letters("0"));
      $assert_equal(String::to_string(1, 2), Letters("1"));

      $assert_equal(String::to_string(133789), Letters("133789"));
      $assert_equal(String::to_string(0), Letters("0"));
      $assert_equal(String::to_string(-512), Letters("-512"));
    }

    void test_idt() {
      InterruptDescriptorTable::Entry *ptr = (InterruptDescriptorTable::Entry*)0xf0f0f0f0f0f0f0f0;
      // InterruptDescriptorTable::IDTDescriptor d = InterruptDescriptorTable::IDTDescriptor(ptr, 0x1010);

      // $assert_equal(String::to_string(d.upper, 2), Letters("1000000010000111100001111000011110000111100001111000011110000"));
      // $assert_equal(String::to_string(d.lower, 2), Letters("1111000011110000000000000000000000000000000000000000000000000000"));

      InterruptDescriptorTable::IDTDescriptor d = InterruptDescriptorTable::IDTDescriptor(ptr, 0xffff);
      $assert_equal(String::to_string(d.data[0], 2), Letters("1111111111111111111100001111000011110000111100001111000011110000"));
      $assert_equal(String::to_string(d.data[1], 2), Letters("1111000011110000000000000000000000000000000000000000000000000000"));
    }

    void test_ports() {
      // TODO
      Port<uint8> p(0x60);

      uint8 c = 0;

      /*
      while(c == 0) c = p.read();

      terminal.print("IN: ");
      terminal.puts(String::to_string(c));
      */
    }

    void run() {
      terminal.puts("BEGIN Tests ----------");
      test_static_array();
      test_string_compare();
      test_number_to_string();
      test_idt();
      test_ports();
      terminal.newline();
      terminal.puts("END Tests ----------");
    }

    void test(Letters name, void (block)()) {
      terminal.print("BEGIN ");
      terminal.print(name);
      terminal.puts(" ----------");

      block();

      terminal.print("END ");
      terminal.print(name);
      terminal.puts(" ----------");
    }
  };
}
