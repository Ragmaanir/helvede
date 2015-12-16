#include <new>

#include "types.hpp"
#include "math.hpp"
#include "numbers.hpp"
#include "static_array.hpp"
#include "allocator.hpp"
#include "kiss_allocator.hpp"
#include "bounded_string.hpp"
#include "version.hpp"
#include "vga_terminal.hpp"
#include "cpu_id.hpp"
#include "logger.hpp"
#include "port.hpp"
#include "pic.hpp"
#include "tests.hpp"

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

    CPUId id;
    id.detect();
    auto idstr = id.idString();

    t.print("Welcome to ");
    t.print("Helvede ", VGATerminal::VGAColor::LightRed, VGATerminal::VGAColor::Black);
    t.puts(VERSION.toString());
    t.print("CPUID: ");
    t.puts(idstr);
    t.puts(String::format(heap_start));

    allocator = KISSAllocator::newEmbeddedIn(heap_start, 1024);
    //allocator = new((void*)heap_start)KISSAllocator((void*)(heap_start + sizeof(KISSAllocator)), 32);
    t.puts(String::format((uint64)allocator));
    allocator->allocate(128);
    t.puts(String::format((uint64)allocator->address()));
    t.puts(String::format((uint64)allocator->top()));

    ChainedPics pics(0x20, 0x28);
    pics.remap();
    //pics.endInterrupt();
    //asm("sti" ::);

    Tests tests(t);

    tests.run();
  }
}

extern "C"
void helvede_kernel_main(Pointer64 heap_start) {
  Helvede::main(heap_start);
}
