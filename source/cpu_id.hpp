namespace Helvede {
  class CPUId {
  public:

    enum class Vendor : uint8 {
      OLDAMD,
      AMD,
      INTEL,
      VIA,
      OLDTRANSMETA,
      TRANSMETA,
      CYRIX,
      CENTAUR,
      NEXGEN,
      UMC,
      SIS,
      NSC,
      RISE
    };

    struct CPUIdMapEntry {
      const char* const id;
      const Vendor  vendor;
    };


    //const CPUIdMapEntry cpuid[1] = {{"AMDisbetter!", Vendor::OLDAMD}};

    // FIXME should be static
    const CPUIdMapEntry cpuidMap[12] = {
      {"AMDisbetter!", Vendor::OLDAMD},
      {"AuthenticAMD", Vendor::AMD},
      {"CentaurHauls", Vendor::CENTAUR},
      {"CyrixInstead", Vendor::CYRIX},
      {"GenuineIntel", Vendor::INTEL},
      {"GenuineTMx86", Vendor::TRANSMETA},
      {"Geode by NSC", Vendor::NSC},
      {"NexGenDriven", Vendor::NEXGEN},
      {"RiseRiseRise", Vendor::RISE},
      {"SiS SiS SiS ", Vendor::SIS},
      {"TransmetaCPU", Vendor::OLDTRANSMETA},
      {"UMC UMC UMC ", Vendor::UMC}
    };

    CPUId() {
      for(char c : _idString) {
        c = '\0';
      }
    }

    void readCPUIdString() {
      uint32 parts[3] = {0,0,0};
      asm(
        "mov eax, 0x0\n"
        "cpuid\n"
        "mov %0, ebx\n"
        "mov %1, ecx\n"
        "mov %2, edx\n"
        : "=r"(parts[0]), "=r"(parts[1]), "=r"(parts[2])
        :
        : "ebx", "ecx", "edx"
      );

      uint32* ptr = (uint32*)_idString;
      ptr[0] = parts[0];
      ptr[1] = parts[2]; // 2 is intended
      ptr[2] = parts[1];
    }

    void detect() {
      readCPUIdString();

      // switch(_idString) {
      //   case "AMDisbetter!": _vendor = Vendor::OldAMD; break;
      //   case "AuthenticAMD": _vendor = Vendor::AMD; break;
      //   case "GenuineIntel": _vendor = Vendor::Intel; break;
      //   case "CentaurHauls": _vendor = Vendor::Via; break;
      //   case "TransmetaCPU": _vendor = Vendor::OldTransmeta; break;
      //   case "GenuineTMx86": _vendor = Vendor::Transmeta; break;
      //   CYRIX        "CyrixInstead",
      //   CENTAUR      "CentaurHauls",
      //   NEXGEN       "NexGenDriven",
      //   UMC          "UMC UMC UMC ",
      //   SIS          "SiS SiS SiS ",
      //   NSC          "Geode by NSC",
      //   RISE         "RiseRiseRise"
      //   default:
      // }
    }

    Letters idString() const {
      return Letters(12, _idString);
    }

    RawString rawIdString() const {
      return _idString;
    }

  private:
    char _idString[13];

  };
}
