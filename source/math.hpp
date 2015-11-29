
namespace Math {
  template<class T>
  constexpr T power(T base, T exponent) {
    return exponent==0 ? 1 : base * power(base, exponent - 1);
  }
};
