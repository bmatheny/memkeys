#ifndef _UTIL_UTIL_TIME_H
#define _UTIL_UTIL_TIME_H

#include <cstdint>
#include <cstring>

extern "C" {
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
}

namespace mckeys {

class UtilTime {
 public:
  static inline uint64_t currentTimeMillis() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (1000L * t.tv_sec) + (t.tv_usec / 1000);
  }
  static inline struct timespec millisToTimespec(long millis) {
    struct timespec interval;
    interval.tv_sec = UtilTime::millisToSeconds(millis);
    interval.tv_nsec = UtilTime::millisToNanos(millis) - UtilTime::secondsToNanos(interval.tv_sec);
    return interval;
  }
  static inline long millisToNanos(long millis) {
    return millis*1000000L;
  }
  static inline long millisToMicros(long millis) {
    return millis*1000L;
  }
  static inline long millisToSeconds(long millis) {
    return millis/1000L;
  }
  static inline long nanosToMillis(long nanos) {
    return nanos/1000000L;
  }
  static inline long microsToMillis(long micros) {
    return micros/1000L;
  }
  static inline long secondsToNanos(long secs) {
    return secs*1000000000L;
  }
};

} // end namespace

#endif
