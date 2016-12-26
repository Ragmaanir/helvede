enum class ComparisonResult {
  LessThan,
  EqualTo,
  GreaterThan
};

template<class Size>
class BoundedString {

public:

  static Size stringLengthOf(RawString chars) {
    Size l = 0;
    while(l < (Size)Number<Size>::Limit::MAX && chars[l]) { l++; };
    return l;
  }

  BoundedString(RawString chars)
    : _length(stringLengthOf(chars)), _characters(chars) { }

  BoundedString(Size l, RawString chars)
    : _length(l), _characters(chars) { }

  RawString rawString() const {
    return _characters;
  }

  Size length() const { return _length; }

  char at(Size index) const {
    // TODO raise if index out of range
    return _characters[index];
  }

  template<class S>
  bool operator==(BoundedString<S> other) const {
    return compare(other) == ComparisonResult::EqualTo;
  }

  template<class S>
  ComparisonResult compare(BoundedString<S> other) const {
    ComparisonResult res = ComparisonResult::EqualTo;
    uint32 min_length = Math::min(_length, other.length());
    uint32 i = 0;
    int32 diff = 0;

    for(; i<min_length; i++) {
      diff = _characters[i] - other.at(i);

      if(diff != 0) {
        break;
      }
    }

    if(diff==0) {
      if(_length < other.length()) {
        res = ComparisonResult::LessThan;
      } else if (_length > other.length()) {
        res = ComparisonResult::GreaterThan;
      }
    } else {
      if(diff < 0) {
        res = ComparisonResult::LessThan;
      } else {
        res = ComparisonResult::GreaterThan;
      }
    }
    return res;
  }

  template<class S>
  void each(void (*block) (const char)) {
    for(Size i = 0; i<_length; i++) {
      block(_characters[i]);
    }
  }

private:
  const Size _length;
  RawString _characters;
};

using Letters = BoundedString<uint8>;
using String = BoundedString<uint16>;
using Text = BoundedString<uint32>;

namespace Helvede {
  namespace String {

    template<class T>
    Letters to_string(T value, uint8 base = 10) {
      // FIXME raise when base invalid
      static char buffer[64];
      static const char *charmap = "0123456789abcdefghijklmnopqrstuvwxyz";
      bool is_negative = value < 0;
      uint64 i = 63;
      value = Number<T>(value).abs();

      do {
        buffer[i] = charmap[value % base];
        value /= base;
        i--;
      } while(value != 0);

      if(is_negative) {
        buffer[i] = '-';
        i--;
      }

      return Letters(63-i, &buffer[i+1]);
    }

    // template<>
    // Letters to_string(uint8* value, uint8 base = 10) {
    // }

    template<>
    Letters to_string(ComparisonResult value, uint8 base) {
      switch(value) {
        case ComparisonResult::EqualTo:
          return Letters(2, "==");
        case ComparisonResult::GreaterThan:
          return Letters(1, ">");
        case ComparisonResult::LessThan:
          return Letters(1, "<");
      }

      return Letters("??");
    }

    template<>
    Letters to_string(Letters value, uint8 base) {
      return value;
    }

  }
}
