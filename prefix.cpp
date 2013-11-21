
#include "prefix.h"
#include <random>

const UniqueIdentifier generateUniqueIdentifier()
{
    u_int64_t mostSignificantBits = arc4random();
    u_int64_t leastSignificantBits = arc4random();
    return (mostSignificantBits << 32) | leastSignificantBits;
}
