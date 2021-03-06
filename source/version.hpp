namespace Helvede {
  struct Version {
    const uint8 major;
    const uint8 minor;
    const uint8 patch;
    const uint8 build;

    const Letters to_string() const {
      return String::to_string(major);
    }
  };

  const Version VERSION = Version({0,0,0,1});
}
