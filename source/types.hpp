typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int int64;
typedef unsigned long long int uint64;

typedef const char * RawString;
typedef uint64 Pointer64;

static_assert(sizeof(int8) == 1, "incorrect size");
static_assert(sizeof(int16) == 2, "incorrect size");
static_assert(sizeof(int32) == 4, "incorrect size");
static_assert(sizeof(int64) == 8, "incorrect size");
