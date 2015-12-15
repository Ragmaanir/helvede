namespace Helvede {
  template<class T, uint32 Size>
  class StaticArray {

    T data[Size];

  public:

    uint32 size() const {
      return Size;
    }

    void set(uint32 at, T value) {
      data[at] = value;
    }

    T operator[](uint32 at) {
      return data[at];
    }

    void each(void (block)(T elem)) {
      for(T e : data) {
        block(e);
      }
    }
  };
}
