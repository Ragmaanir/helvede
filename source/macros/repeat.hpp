
#define $$concat(a, ...) $$concat_f(a, __VA_ARGS__)
#define $$concat_f(a, ...) a ## __VA_ARGS__
// #define $$empty(...)
// #define $repeat_f() $repeat
// #define $repeat(n, exp) exp && $$eval($repeat_f)()(n-1, exp)
// #define $$eval(...) __VA_ARGS__ $$empty()
// #define $$eval_f(f, args) (f ## args) $$empty()

#define $$if(cond) $$concat($$_if_, cond)
#define $$_if_0(c, ...) __VA_ARGS__
#define $$_if_1(c, ...) c

#define $$is_zero(n) $$concat($$is_zero)

// #define $redefine(var, i)
//   \#define #var #i

#define $decrement(i) $$concat($_dec_, i)
#define $_dec_10 9
#define $_dec_9 8
#define $_dec_8 7
#define $_dec_7 6
#define $_dec_6 5
#define $_dec_5 4
#define $_dec_4 3
#define $_dec_3 2
#define $_dec_2 1
#define $_dec_1 0
#define $_dec_0 0

//#define $repeat(n, exp) $repeat_## ($decrement(n)(exp))
// #define $repeat(n, exp) $$if($$is_zero(n))( $repeat($decrement(n), (exp)) )

#define $repeat(n) $$concat($repeat_, n)
#define $repeat_1(exp) exp;
#define $repeat_2(exp) exp; $repeat_1(exp)
#define $repeat_3(exp) exp; $repeat_2(exp)
#define $repeat_4(exp) exp; $repeat_3(exp)
#define $repeat_5(exp) exp; $repeat_4(exp)
#define $repeat_6(exp) exp; $repeat_5(exp)
#define $repeat_7(exp) exp; $repeat_6(exp)
#define $repeat_8(exp) exp; $repeat_7(exp)
#define $repeat_9(exp) exp; $repeat_8(exp)
#define $repeat_10(exp) exp; $repeat_9(exp)
#define $repeat_11(exp) exp; $repeat_10(exp)

//#define $rep(n, exp) (n==1) ? exp : $rep_r(n, exp)
//#define $rep_r(n, exp) $rep(n-1, exp)
//#define $$delay(name, ...) (($ ## name))(__VA_ARGS__)

void macros() {
  // $$if(1 > 0)(1, xxx);

  int i = 0;
  $repeat(6)(
    i += 1;
  )
  // Helvede::Dbg::put(10, 10, i);
}
