#ifndef OPTIONS_H
#define OPTIONS_H

#define no_argument 0
#define required_argument 1
#define optional_argument 2

#define SIZE_BITS(type) (1 << (8*sizeof(type)))
#define REQUIRE_UINT(name, value, type) do { \
    if (value <= 0) { \
        throw std::range_error(name " must be >= 0"); \
    } else if (value >= SIZE_BITS(type)) { \
        throw std::range_error(name " must be < 65536"); \
    } \
} while(0);

#endif
