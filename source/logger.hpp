namespace Helvede {
  class Logger {

  public:

    enum struct Level {
      DEBUG = 0,
      INFO,
      WARN,
      ERROR,
      FATAL
    };

    Logger(Level level) : current_level(level) {}

    void log(Level level, RawString message) {

    }

  private:

    Level current_level;

  };
}
