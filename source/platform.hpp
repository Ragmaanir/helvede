namespace Helvede {
  namespace Platform {
    enum class Endianness {
      UNKNOWN,
      LITTLE,
      BIG
    };

    struct EndiannessDetector {
      union {
        uint8 bytes[8];
        uint64 qword;
      } data;

      EndiannessDetector() {
        data.qword = 0x0102030405060708;
      }
    };

    Endianness endianness() {
      EndiannessDetector endianness_detector;

      if(endianness_detector.data.bytes[0] == 0x1) {
        return Endianness::BIG;
      } else if(endianness_detector.data.bytes[0] == 0x8) {
        return Endianness::LITTLE;
      } else {
        return Endianness::UNKNOWN;
      }
    }

    RawString endianness_name() {
      switch(Platform::endianness()) {
        case Platform::Endianness::LITTLE:
          return "LITTLE";
        case Platform::Endianness::BIG:
          return "BIG";
        default:
          return "UNKNOWN";
      }
    }
  }
}
