#include "util/stats.h"
#include <algorithm>

namespace mctop {

using namespace std;

Stats::Stats(const Config * config)
    : config(config),
      _collection(),
      _mutex()
{}
Stats::~Stats() {}

// This will want to take a sort mode (what value to sort on) and a sort order
// (asc,desc) as arguments
template<class T>
priority_queue<Stat,vector<Stat>,T> Stats::getLeaders(const uint16_t size) {
  priority_queue<Stat, vector<Stat>, T> pq;
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

void Stats::increment(const string &key, const uint32_t size) {
  ssize_t _key = Stat::hashKey(key);
  _mutex.lock();
  StatCollection::const_iterator it = _collection.find(_key);
  if (it != _collection.end()) {
    Stat stat = it->second;
    stat.setSize(size);
    stat.increment();
  } else {
    _collection.insert(it, StatCollection::value_type(_key, Stat(key, size)));
  }
  _mutex.unlock();
}

// FIXME this should be in getLeaders I think
void Stats::prune(const double threshold) {
  _mutex.lock();
  StatCollection::iterator it = _collection.begin();
  while (it != _collection.end()) {
    Stat stat = it->second;
    if (stat.requestRate() < config->getDiscardThreshold()) {
      _collection.erase(it++);
    } else {
      ++it;
    }
  }
  // FIXME not sure what this rehash param does
  _collection.rehash(0);
  _mutex.unlock();
}

} // end namespace mctop
