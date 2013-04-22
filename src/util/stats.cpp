#include "common.h"

namespace mckeys {

using namespace std;

Stats::Stats(const Config *config, mqueue<Elem> *mq)
    : config(config),
      barrier(mq),
      _collection(),
      _mutex(),
      logger(Logger::getLogger("stats")),
      state()
{}
Stats::~Stats() {
  if (state.checkAndSet(state_STOPPING, state_TERMINATED)) {
    logger->info(CONTEXT, "Stats successfully shut down");
  } else {
    logger->error(CONTEXT, "Stats not successfully shut down");
  }
  delete logger;
}

void Stats::start() {
  if (state.checkAndSet(state_NEW, state_RUNNING)) {
    logger->info(CONTEXT, "Starting stats engine");
    reaper_thread = thread(&Stats::prune, this);
    poller_thread = thread(&Stats::collect, this);
  } else {
    logger->warning(CONTEXT, "Stats engine already started");
  }
}

void Stats::shutdown() {
  if (state.checkAndSet(state_RUNNING, state_STOPPING)) {
    // FIXME we should wrap these joins in a timer
    logger->info(CONTEXT, "Stopping stats engine");
    reaper_thread.join();
    logger->info(CONTEXT, "Repear thread dead");
    poller_thread.join();
    logger->info(CONTEXT, "Poller thread dead");
  } else {
    logger->warning(CONTEXT, "Stats engine already stopping");
  }
}

void Stats::increment(const string &key, const uint32_t size) {
  ssize_t _key = Stat::hashKey(key);
  _mutex.lock();
  StatCollection::iterator it = _collection.find(_key);
  if (it != _collection.end()) {
    Stat stat = it->second;
    // FIXME this should probably only be done periodically, not every time
    // since it is unlikely to change very often
    stat.setSize(size);
    stat.increment();
#ifdef _DEBUG
    uint64_t ssize = stat.getCount();
    if (ssize >= 2) {
      logger->trace(CONTEXT,
                    "Incremented stat: %s, %d -> %ld", key.c_str(), size, ssize);
    }
#endif
    _collection[_key] = stat;
  } else {
    _collection.insert(it, StatCollection::value_type(_key, Stat(key, size)));
  }
  _mutex.unlock();
}

void Stats::printStats(const uint16_t size) {
  deque<Stat> q = getLeaders<SortByCount>(size);
  uint32_t qsize = q.size();
  if (qsize > 0) {
    cout << setw(110) << "Key" << ", ";
    cout << setw(10) << "Count" << ", ";
    cout << setw(10) << "Elapsed" << ", ";
    cout << setw(10) << "Rate" << ", ";
    cout << setw(10) << "Size" << ", ";
    cout << setw(10) << "BW" << endl;
  }
  for (deque<Stat>::iterator it = q.begin(); it != q.end(); ++it) {
    Stat stat = *it;
    cout << setw(110) << stat.getKey() << ", ";
    cout << setw(10) << stat.getCount() << ", ";
    cout << setw(10) << stat.elapsed() << ", ";
    cout << setw(10) << std::setprecision(2) << stat.requestRate() << ", ";
    cout << setw(10) << stat.getSize() << ", ";
    cout << setw(10) << std::setprecision(2) << stat.bandwidth() << endl;
  }
}

///////////////////////////////////////////////////////////////////////////////
//                      Protected Methods                                    //
///////////////////////////////////////////////////////////////////////////////
void Stats::collect() {
  static Backoff backoff;
  static uint64_t backoffMs;
  static struct timespec waitTime;
  logger->info(CONTEXT, "Starting stats collection");
  Elem e;
  while (state.isRunning()) {
    if (barrier->consume(e)) { // got one
#ifdef _DEBUG
      logger->trace(CONTEXT,
                   "Consumed stat: %s, %d", e.first.c_str(), e.second);
#endif
      increment(e.first, e.second);
      if (backoffMs > 0) {
        backoffMs = 0;
        backoff.reset();
      }
    } else { // did not get one
      backoffMs = backoff.getNextBackOffMillis();
      waitTime = UtilTime::millisToTimespec(backoffMs);
#ifdef _DEVEL
      logger->trace(CONTEXT,
                    "No stat to consume, will sleep %lu ms", backoffMs);
#endif
    }
    if (backoffMs > 0) {
      nanosleep(&waitTime, NULL);
    }
  }
  logger->info(CONTEXT, "Stats collect thread stopped");
}

// FIXME this should be in getLeaders I think?
void Stats::prune() {
  static const double threshold = config->getDiscardThreshold();
  int size_pre = 0, size_post = 0;
  StatCollection::iterator it;
  logger->info(CONTEXT, "Starting prune with threshold %0.2f", threshold);
  while (state.isRunning()) {
    _mutex.lock();
    //printStats(50);
    it = _collection.begin();
    size_pre = _collection.size();
    while (it != _collection.end()) {
      Stat stat = it->second;
      if (stat.requestRate() < threshold) {
        _collection.erase(it++);
      } else {
        ++it;
      }
    }
    // FIXME not sure what this rehash param does
    _collection.rehash(0);
    size_post = _collection.size();
    logger->debug(CONTEXT,
                 "Stats collection size: %d -> %d", size_pre, size_post);
    _mutex.unlock();
    // FIXME this should sleep for the UI refresh interval
    sleep(5);
  }
  logger->info(CONTEXT, "Stats prune thread stopped");
}

} // end namespace
