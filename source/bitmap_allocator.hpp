namespace Helvede {
  class AllocatorStats {
  public:
    uint32 allocationCount;
    uint32 freeCount;
  };

  class Allocator {
  public:
    virtual ~Allocator() = 0;

    virtual uint32 capacity() const = 0;
    virtual uint32 allocatedBytes() const;
    virtual uint32 freeBytes() const = 0;

    virtual const AllocatorStats stats() const = 0;

    virtual void* allocate(uint32 size) = 0;
    virtual void deallocate(void* pointer) = 0;
  };

  /*
  class BitmapAllocator : public Allocator {
    static const uint64 ALL_FREE = ~0;

    template<uint32 Bits>
    class Bitmap {
      static_assert(Bits > 64 && Bits % 64 == 0, "ERROR: Bits has to be > 0 and a multiple of 64");

      uint32 markedBits;
      uint64 blocks[Bits / 64];

    public:

      Bitmap() {
        markedBits = 0;
      }

      void clear() const {
        for(uint32 i=0; i < Blocks/64; i++) {
          blocks[i] = 0;
        }
      }

      uint32 mark() {
        assert(markedBits == Bits);
        uint32 index = freeBitIndex();
        markBit(index);
        return index;
      }

      uint32 totalMemory() const {
        return Blocks * BlockSize;
      }

      uint32 countFreeBlocks() const {
        uint32 freeBlocks = 0;
        for(uint32 i=0; i < Blocks/64; i++) {
          freeBlocks += Uin32(freeBits[i]).bitSet();
        }

        return freeBlocks;
      }

      uint32 usedBlocksCount() const {
        return Blocks - freeBlocksCount();
      }

      bool isFull() const {
        return freeBlocksCount == 0;
        // for(uint32 i=0; i < Blocks/64; i++) {
        //   if(freeBits[i] != 0) {
        //     return false;
        //   }
        // }
        //
        // return true;
      }
    };

    // template<uint8 Blocks, uint8 BlockSize>
    // class Bitmap {
    //   static_assert(Blocks > 64 && Blocks % 64 == 0, "ERROR: Blocks has to be > 0 and a multiple of 64");
    //
    //   uint32 freeBlocksCount;
    //   uint64 freeBits[Blocks / 64];
    //   uint8 memory[Blocks * BlockSize];
    //
    // public:
    //
    //   Bitmap(uint8 *at) : memory(at) {
    //     freeBlocksCount = Blocks;
    //   }
    //
    //   void clear() const {
    //     for(uint32 i=0; i < Blocks/64; i++) {
    //       freeBits[i] = ALL_FREE;
    //     }
    //   }
    //
    //   uint32* allocate(uint32 bytes) {
    //     int32 requiredBlocks = bytes / BlockSize;
    //
    //     if(freeBlocksCount < requiredBlocks) {
    //       return NULL;
    //     }
    //   }
    //
    //   uint32 totalMemory() const {
    //     return Blocks * BlockSize;
    //   }
    //
    //   uint32 countFreeBlocks() const {
    //     uint32 freeBlocks = 0;
    //     for(uint32 i=0; i < Blocks/64; i++) {
    //       freeBlocks += Uin32(freeBits[i]).bitSet();
    //     }
    //
    //     return freeBlocks;
    //   }
    //
    //   uint32 usedBlocksCount() const {
    //     return Blocks - freeBlocksCount();
    //   }
    //
    //   bool isFull() const {
    //     return freeBlocksCount == 0;
    //     // for(uint32 i=0; i < Blocks/64; i++) {
    //     //   if(freeBits[i] != 0) {
    //     //     return false;
    //     //   }
    //     // }
    //     //
    //     // return true;
    //   }
    // };
  };
  */
}
