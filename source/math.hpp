
namespace Math {
  // template<typename T>
  // constexpr T PI = T(3.14159265358979323846);

  template<class T>
  constexpr T power(T base, T exponent) {
    return exponent==0 ? 1 : base * power(base, exponent - 1);
  }

  template<class T>
  constexpr T min(T left, T right) {
    return left <= right ? left : right;
  }
};

static_assert(Math::power(1,0) == 1, "1^0 != 1");
static_assert(Math::power(1,2) == 1, "1^2 != 1");
static_assert(Math::power(2,10) == 1024, "2^10 != 1024");
