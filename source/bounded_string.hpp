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

typedef BoundedString<uint8> Letters;
typedef BoundedString<uint16> String;
typedef BoundedString<uint32> Text;
