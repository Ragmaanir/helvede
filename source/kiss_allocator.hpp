namespace Helvede {

  class KISSAllocator /*: public Allocator*/ {
    Pointer64 _address;
    Pointer64 _top;
    uint32 _capacity;

  public:

    static KISSAllocator* newEmbeddedIn(Pointer64 address, uint32 capacity) {
      KISSAllocator* instance;
      instance = new((void*)address)KISSAllocator(address + sizeof(KISSAllocator), capacity);
      return instance;
    }

    KISSAllocator(Pointer64 address, uint32 capacity)
      : _address(address), _top(address), _capacity(capacity) {

    }
    ~KISSAllocator() {

    }

    uint32 capacity()       const { return _capacity; }
    uint32 allocatedBytes() const { return 0; }
    uint32 freeBytes()      const { return 0; }

    const AllocatorStats stats() const {
      return AllocatorStats();
    }

    void* allocate(uint32 size) {
      Pointer64 addr = _top;
      _top = _top + size;
      return (void*)addr;
    }
    void deallocate(void* pointer) {

    }

    const void* address() const {
      return (void*)_address;
    }

    const void* top() const {
      return (void*)_top;
    }
  };

}
