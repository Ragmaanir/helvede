#include <new>

#include "types.hpp"
#include "math.hpp"
#include "numbers.hpp"
#include "allocator.hpp"
#include "kiss_allocator.hpp"
#include "bounded_string.hpp"
#include "vga_terminal.hpp"
#include "logger.hpp"
#include "port.hpp"

/*
void* operator new (long unsigned int size, void* ptr) noexcept
{
  return ptr;
}

void operator delete (void* ptr) noexcept
{

}
*/

void operator delete (void* ptr) noexcept
{

}

namespace Helvede {
  KISSAllocator *allocator;

  /*void * operator new (uint32 size, void *memory) throw() {

  }*/

  // template<class T>
  // T* instantiate(void* mem, T data) {
  //   T* item = (T*)mem;
  //   *item = data;
  //   return item;
  // }

  // void* operator new(void* mem, T data) {
  //   T* item = (T*)mem;
  //   *item = data;
  //   return item;
  // }

  void main(Pointer64 heap_start) {
    //allocator = instantiate((void*)0xf000, KISSAllocator(heap, 32));

    VGATerminal t = VGATerminal();

    t.clear();
    t.puts("Welcome to Helvede");
    t.puts(Int64(heap_start).toString());

    allocator = KISSAllocator::newEmbeddedIn(heap_start, 1024);
    //allocator = new((void*)heap_start)KISSAllocator((void*)(heap_start + sizeof(KISSAllocator)), 32);
    t.puts(Int64((uint64)allocator).toString());
    allocator->allocate(128);
    t.puts(Int64((uint64)allocator->address()).toString());
    t.puts(Int64((uint64)allocator->top()).toString());
  }
}

extern "C"
void helvede_kernel_main(Pointer64 heap_start) {
  Helvede::main(heap_start);
}
