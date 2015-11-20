class String {

public:

  static uint8 string_length_of(RawString chars) {
    uint8 l = 0;
    while(l < 255 && chars[l]) { l++; };
    return l;
  }

  String(RawString chars) : length(string_length_of(chars)), characters(chars) {
  }

  String(uint8 l, RawString chars) : length(l), characters(chars) {
  }

private:
  const uint8 length;
  RawString characters;
  
};
