#ifndef _UTIL_STAT_H
#define _UTIL_STAT_H

#include <cstdint>
#include <cstdatomic>
#include <functional>
#include <unordered_map>
#include <string>

#include "util/util_time.h"

namespace mctop {

/**
 * A Stat is part of a collection of stats.
 * Each stat has a unique key, and a possibly changing size.
 * Every time you see this key, call increment on the associated stat.
 * You can then easily determine the requestRate, associated bandwidth, and the
 * total seen count.
 *
 * Calls wrt count() and size are atomic. Size may be mutated.
 *
 * Also see Stats
 */
class Stat {
 public:
  static inline size_t hashKey(const std::string &key) {
    std::hash<std::string> hash_fn;
    return hash_fn(key);
  }

  Stat(const std::string &key, const uint32_t size);
  ~Stat();
  // std::atomic member variables require that your class implement a copy
  // constructor and the assignment operator, if you want to use the Stat class
  // with most STL collections
  Stat(const Stat& stat);
  Stat& operator=(const Stat& rhs);

  uint64_t getCreated() const;
  const std::string getKey() const;
  uint32_t getSize() const;
  void setSize(const uint32_t sz);

  double bandwidth(const uint64_t e) const;
  inline double bandwidth() const {
    return bandwidth(elapsed());
  }

  uint64_t getCount() const {
    return _count.load();
  }

  inline uint64_t elapsed() const {
    return UtilTime::currentTimeMillis() - _created;
  }

  void increment();

  double requestRate(const uint64_t e) const;
  inline double requestRate() const {
    return requestRate(elapsed());
  }

 private:
  std::string key;
  uint64_t _created;
  mutable std::atomic_uint_fast32_t _size;
  mutable std::atomic_uint_fast64_t _count;
};

typedef std::unordered_map<ssize_t, Stat> StatCollection;
typedef std::pair<ssize_t, Stat> StatPair;

} // end namespace mctop

#endif
