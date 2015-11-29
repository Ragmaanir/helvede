template<class Size>
class BoundedString;

template<class T>
struct Number {
  const T value;
  // FIXME hardcoded int64
  enum class Limit : int64 {
    BITS = (int64)sizeof(T)*8,
    BASE = Math::power((int64)2, BITS-1),
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

  // const BoundedString<uint8> toString() const {
  RawString toString() const {
    static T divisors[9] = { Math::power(10, 8), Math::power(10, 7), Math::power(10, 6), Math::power(10, 5), Math::power(10, 4), Math::power(10, 3), Math::power(10, 2), 10, 1};
    static char* buffer = "000000000";
    T v = abs();
    for(uint32 i = 0; v != 0 && i < 9; i++) {
      buffer[i] = '0' + v / divisors[i];
      v = v % divisors[i];
    }

    // auto x = BoundedString<uint8>(9, buffer)
    // return x;
    return buffer;
  }
};

typedef Number<int32> Int32;
typedef Number<int16> Int16;
