namespace Helvede {

  class AllocatorStats {
  public:
    uint32 allocationCount;
    uint32 freeCount;
  };

  /*class Allocator {
  public:
    virtual ~Allocator() {};

    virtual uint32 capacity() const = 0;
    virtual uint32 allocatedBytes() const;
    virtual uint32 freeBytes() const = 0;

    virtual const AllocatorStats stats() const = 0;

    virtual void* allocate(uint32 size) = 0;
    virtual void deallocate(void* pointer) = 0;
  };*/

}
