namespace Helvede {
  template<class E, class C>
  void each(C coll, void (block)(E elem));

  template<class E, E[]>
  void each(E[] coll, void (block)(E elem)) {
    for(E e : coll) {
      block(e);
    }
  }
}
