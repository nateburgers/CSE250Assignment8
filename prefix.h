#ifndef PREFIX_H
#define PREFIX_H

#include <cstdint>

typedef void* Reference;
typedef unsigned char Char;
typedef int64_t Int;
typedef uint64_t UInt;

typedef long double Float;

typedef bool Bool;
#define NO false
#define YES true

#define nil 0

#define UNDEFINED NO
#define NOT_IMPLEMENTED NO
#define IMPLEMENTED YES

typedef UInt UniqueIdentifier;
const UniqueIdentifier generateUniqueIdentifier();

#endif // PREFIX_H
