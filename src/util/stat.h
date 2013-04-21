#ifndef _UTIL_STAT_H
#define _UTIL_STAT_H

#include <ctime>
#include <cstdint>
#include <cstdatomic>
#include <functional>
#include <unordered_map>
#include <string>

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

  static inline uint64_t now() {
    time_t t = time(NULL);
    return (1000L * t);
  }

  Stat(const std::string &key, const uint32_t size);
  ~Stat();
  Stat(const Stat &stat); // required for StatCollection

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
    return Stat::now() - _created;
  }

  void increment();

  double requestRate(const uint64_t e) const;
  inline double requestRate() const {
    return requestRate(elapsed());
  }

 protected:

 private:

  std::string key;
  uint64_t _created;
  mutable std::atomic_uint_fast32_t _size;
  mutable std::atomic_uint_fast64_t _count;
};

typedef std::unordered_map<ssize_t, Stat> StatCollection;

} // end namespace mctop

#endif
