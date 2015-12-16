template<class Size>
class BoundedString {

public:

  enum class ComparisonResult {
    LessThan,
    EqualTo,
    GreaterThan
  };

  static Size stringLengthOf(RawString chars) {
    Size l = 0;
    while(l < Number<Size>::Limit::MAX && chars[l]) { l++; };
    return l;
  }

  BoundedString(RawString chars) : _length(stringLengthOf(chars)), _characters(chars) {
  }

  BoundedString(Size l, RawString chars) : _length(l), _characters(chars) {
  }

  RawString rawString() const {
    return _characters;
  }

  Size length() const {
    return _length;
  }

  template<class S>
  bool operator==(BoundedString<S> other) const {
    return compare(other) == ComparisonResult::EqualTo;
  }

  template<class S>
  ComparisonResult compare(BoundedString<S> other) const {
    return ComparisonResult::EqualTo; // TODO
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
    // const BoundedString<uint8> toString() const {
    // TODO maybe move this to BoundedString::format or so?
    // FIXME return BoundedString<uint8>
    //RawString toString() const {
    template<class T>
    Letters format(Number<T> value) {
      static const uint64 divisors[10] = {
        Math::power(10, 9), Math::power(10, 8), Math::power(10, 7),
        Math::power(10, 6), Math::power(10, 5), Math::power(10, 4),
        Math::power(10, 3), Math::power(10, 2), 10, 1
      };
      static char* buffer = "          "; // FIXME allocate memory
      uint64 v = value.abs();
      int64 first_digit_at = -1;

      for(uint32 i = 0; i < 10; i++) {
        uint64 digit = v / divisors[i];
        v = v % divisors[i];

        if(first_digit_at == -1 && (digit > 0 || i==9)) {
          first_digit_at = i;
        }
        if(first_digit_at > -1) {
          buffer[i] = '0' + digit;
        } else {
          buffer[i] = ' ';
        }
      }

      return Letters(10 - first_digit_at, &buffer[first_digit_at]);
    }

    template<class T>
    Letters format(T value) {
      static const uint64 divisors[10] = {
        Math::power(10, 9), Math::power(10, 8), Math::power(10, 7),
        Math::power(10, 6), Math::power(10, 5), Math::power(10, 4),
        Math::power(10, 3), Math::power(10, 2), 10, 1
      };
      static char* buffer = "          "; // FIXME allocate memory
      uint64 v = Number<T>(value).abs();
      int64 first_digit_at = -1;

      for(uint32 i = 0; i < 10; i++) {
        uint64 digit = v / divisors[i];
        v = v % divisors[i];

        if(first_digit_at == -1 && (digit > 0 || i==9)) {
          first_digit_at = i;
        }
        if(first_digit_at > -1) {
          buffer[i] = '0' + digit;
        } else {
          buffer[i] = ' ';
        }
      }

      return Letters(10 - first_digit_at, &buffer[first_digit_at]);
    }
  }
}
