template<class Size>
class BoundedString {

public:

  static Size string_length_of(RawString chars) {
    Size l = 0;
    while(l < Number<Size>::Limit::MAX && chars[l]) { l++; };
    return l;
  }

  BoundedString(RawString chars) : length(string_length_of(chars)), characters(chars) {
  }

  BoundedString(Size l, RawString chars) : length(l), characters(chars) {
  }

private:
  const Size length;
  RawString characters;

};
