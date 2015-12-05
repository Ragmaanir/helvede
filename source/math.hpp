
namespace Math {
  template<class T>
  constexpr T power(T base, T exponent) {
    return exponent==0 ? 1 : base * power(base, exponent - 1);
  }
};

static_assert(Math::power(1,0) == 1, "1^0 != 1");
static_assert(Math::power(1,2) == 1, "1^2 != 1");
static_assert(Math::power(2,10) == 1024, "2^10 != 1024");
