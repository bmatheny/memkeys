#ifndef _UTIL_STATS_H
#define _UTIL_STATS_H

#include <algorithm>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include "config.h"
#include "util/mqueue.h"
#include "util/stat.h"
#include "util/state.h"

namespace mctop {

class SortByRequestRate {
 public:
  bool operator() (const Stat &first, const Stat &second) {
    return (first.requestRate() <= second.requestRate());
  }
};

typedef std::pair<std::string,uint32_t> Elem;

// Keep track of a collection of Stat items
class Stats
{
 public:
  Stats(const Config *config, mqueue<Elem> *mq);
  ~Stats();

  void start();
  void shutdown();

  void increment(const std::string &key, const uint32_t size);
  // This will want to take a sort mode (what value to sort on) and a sort order
  // (asc,desc) as arguments
  template<class T>
  std::priority_queue<Stat,std::vector<Stat>,T> getLeaders(const uint16_t size)
  {
    std::priority_queue<Stat, std::vector<Stat>, T> pq;
    for (StatCollection::iterator it = _collection.begin();
         it != _collection.end(); it++)
    {
      pq.push(it->second);
      if (pq.size() > size) {
        pq.pop();
      }
    }
    return pq;
  }

 protected:
  // These are run in threads
  void collect();
  void prune();

 private:
  const Config * config;
  mqueue<Elem> * barrier;
  StatCollection _collection;
  mutable std::mutex _mutex;
  LoggerPtr logger;
  State state;
  std::thread reaper_thread;
  std::thread poller_thread;
};


}

#endif
