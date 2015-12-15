template<class Size>
class BoundedString;

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

  // const BoundedString<uint8> toString() const {
  RawString toString() const {
    //static const T divisors[10] = { Math::power(10, 9), Math::power(10, 8), Math::power(10, 7), Math::power(10, 6), Math::power(10, 5), Math::power(10, 4), Math::power(10, 3), Math::power(10, 2), 10, 1};
    static const uint64 divisors[10] = { Math::power(10, 9), Math::power(10, 8), Math::power(10, 7), Math::power(10, 6), Math::power(10, 5), Math::power(10, 4), Math::power(10, 3), Math::power(10, 2), 10, 1};
    static char* buffer = "0000000000"; // FIXME allocate memory
    T v = abs();
    for(uint32 i = 0; i < 10; i++) {
      buffer[i] = '0' + v / divisors[i];
      v = v % divisors[i];
    }

    // auto x = BoundedString<uint8>(9, buffer)
    // return x;
    return buffer;
  }
};

typedef Number<int64> Int64;
typedef Number<int32> Int32;
typedef Number<int16> Int16;
typedef Number<int8> Int8;

typedef Number<uint64> Uint64;
typedef Number<uint32> Uint32;
typedef Number<uint16> Uint16;
typedef Number<uint8> Uint8;
