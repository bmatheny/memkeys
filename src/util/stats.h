#ifndef _UTIL_STATS_H
#define _UTIL_STATS_H

#include <string>
#include <mutex>
#include <queue>
#include <vector>
#include "config.h"
#include "util/stat.h"

namespace mctop {

class SortByRequestRate {
 public:
  bool operator() (const Stat &first, const Stat &second) {
    return (first.requestRate() <= second.requestRate());
  }
};

// Keep track of a collection of Stat items
class Stats
{
 public:
  Stats(const Config *config);
  ~Stats();

  void increment(const std::string &key, const uint32_t size);
  void prune(const double threshold);
  template<class T>
  std::priority_queue<Stat,std::vector<Stat>,T> getLeaders(const uint16_t size);

 private:
  const Config * config;
  StatCollection _collection;
  mutable std::mutex _mutex;
};


}

#endif
