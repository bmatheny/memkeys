#ifndef _UTIL_STATS_H
#define _UTIL_STATS_H

#include <algorithm>
#include <deque>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include "config.h"
#include "util/mqueue.h"
#include "util/stat.h"
#include "util/state.h"

namespace mckeys {

typedef enum {
  sort_DESC, sort_ASC
} SortOrder;

typedef enum {
  mode_CALLS, mode_SIZE, mode_REQRATE, mode_BANDWIDTH
} SortMode;

class SortByCount {
 public:
  bool operator() (const Stat &first, const Stat &second) {
    return (first.getCount() > second.getCount());
  }
};
class SortBySize {
 public:
  bool operator() (const Stat &first, const Stat &second) {
    return (first.getSize() > second.getSize());
  }
};
class SortByReqRate {
 public:
  bool operator() (const Stat &first, const Stat &second) {
    return (first.requestRate() > second.requestRate());
  }
};
class SortByBandwidth {
 public:
  bool operator() (const Stat &first, const Stat &second) {
    return (first.bandwidth() > second.bandwidth());
  }
};

typedef std::pair<std::string,uint32_t> Elem;

// Keep track of a collection of Stat items
class Stats
{
 public:
  static std::string getSortModeString(const SortMode& sortMode);
  static std::string getSortOrderString(const SortOrder& sortOrder);

  Stats(const Config *config, mqueue<Elem> *mq);
  ~Stats();

  void start();
  void shutdown();

  void increment(const std::string &key, const uint32_t size);
  void printStats(const uint16_t size);
  uint32_t getStatCount();

  // This will want to take a sort mode (what value to sort on) and a sort order
  // (asc,desc) as arguments
  // FIXME this sucks big time
  template<class T>
  std::deque<Stat> getLeaders() {
    std::priority_queue<Stat, std::vector<Stat>, T> pq;
    std::deque<Stat> holder;
    _mutex.lock();
    for (StatCollection::iterator it = _collection.begin();
         it != _collection.end(); ++it)
    {
      pq.push(it->second);
    }
    _mutex.unlock();
    while(!pq.empty()) {
      holder.push_front(pq.top());
      pq.pop();
    }
    return holder;
  }
  std::deque<Stat> getLeaders(const SortMode mode, const SortOrder order);

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
