template<class T>
struct Number {
  const T value;
  // FIXME hardcoded int64
  enum class Limit : uint64 {
    BITS = (uint64)sizeof(T)*8,
    BASE = Math::power((uint64)2, BITS-1),
    MAX = BASE-1,
    MIN = -BASE+1
  };

  Number(T v) : value(v) {}

  inline const Number operator+(Number other) const {
    return other.value + value;
  }

  inline bool operator==(Number other) const {
    return other.value == value;
  }

  inline T abs() const {
    return value >= 0 ? value : -value;
  }

  inline T bitsSet() const {
    T v = value;
    for (uint32 c = 0; v; c++) {
      v &= v - 1; // clear the least significant bit set
    }

    return v;
  }

  // // const BoundedString<uint8> toString() const {
  // // TODO maybe move this to BoundedString::format or so?
  // // FIXME return BoundedString<uint8>
  // //RawString toString() const {
  // Letters toString() {
  //   static const uint64 divisors[10] = {
  //     Math::power(10, 9), Math::power(10, 8), Math::power(10, 7),
  //     Math::power(10, 6), Math::power(10, 5), Math::power(10, 4),
  //     Math::power(10, 3), Math::power(10, 2), 10, 1
  //   };
  //   static char* buffer = "          "; // FIXME allocate memory
  //   uint64 v = abs();
  //   int64 first_digit_at = -1;
  //
  //   for(uint32 i = 0; i < 10; i++) {
  //     uint64 digit = v / divisors[i];
  //     v = v % divisors[i];
  //
  //     if(first_digit_at == -1 && (digit > 0 || i==9)) {
  //       first_digit_at = i;
  //     }
  //     if(first_digit_at > -1) {
  //       buffer[i] = '0' + digit;
  //     } else {
  //       buffer[i] = ' ';
  //     }
  //   }
  //
  //   return Letters(10 - first_digit_at, &buffer[first_digit_at]);
  // }
};

using Int64 = Number<int64>;
using Int32 = Number<int32>;
using Int16 = Number<int16>;
using Int8  = Number<int8>;

using Uint64 = Number<uint64>;
using Uint32 = Number<uint32>;
using Uint16 = Number<uint16>;
using Uint8  = Number<uint8>;
