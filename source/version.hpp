namespace Helvede {
  struct Version {
    const uint8 major;
    const uint8 minor;
    const uint8 patch;
    const uint8 build;

    Letters toString() const {
      static char str[4*3];
      return String::format(major);
    }
  };

  const Version VERSION = Version({0,0,0,1});
}
