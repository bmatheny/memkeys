#ifndef _UTIL_OPTIONS_H
#define _UTIL_OPTIONS_H

#include <sstream>

#ifndef no_argument
#define no_argument 0
#endif

#ifndef required_argument
#define required_argument 1
#endif

#ifndef optional_argument
#define optional_argument 2
#endif

#define USIZE_BITS(type) ((1L << (8*sizeof(type))) - 1)
#define REQUIRE_UINT(name, value, type) do { \
    uint64_t usize = USIZE_BITS(type); \
    if (value <= 0) { \
        throw std::range_error(name " must be >= 0"); \
    } else if ((uint64_t)value > usize) { \
        std::ostringstream oss; \
        oss << name << " must be <= " << usize; \
        throw std::range_error(oss.str()); \
    } \
} while(0);

#endif
