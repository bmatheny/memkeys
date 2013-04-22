#ifndef _UTIL_UTIL_TIME_H
#define _UTIL_UTIL_TIME_H

#include <cstdint>

extern "C" {
#include <sys/time.h>
}

namespace mctop {

class UtilTime {
 public:
  static inline uint64_t currentTimeMillis() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (1000L * t.tv_sec) + (t.tv_usec / 1000);
  }
};

} // end namespace mctop

#endif
