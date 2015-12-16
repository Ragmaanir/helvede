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

};

using Int64 = Number<int64>;
using Int32 = Number<int32>;
using Int16 = Number<int16>;
using Int8  = Number<int8>;

using Uint64 = Number<uint64>;
using Uint32 = Number<uint32>;
using Uint16 = Number<uint16>;
using Uint8  = Number<uint8>;
