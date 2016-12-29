//#define $assert_op(l, op, r) if(!(l op r)) {  }
#define $assert(e) assert(e, #e)

void assert(bool exp, const char* const msg) {
  if(!exp){
    VGATerminal t(23, 30);
    t.puts(msg, TermColorings::WhiteOnRed);
  }
}

