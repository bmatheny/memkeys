#include "common.h"

namespace mctop {

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
  StatCollection::const_iterator it = _collection.find(_key);
  if (it != _collection.end()) {
    Stat stat = it->second;
    // FIXME this should probably only be done periodically, not every time
    // since it is unlikely to change very often
    stat.setSize(size);
    stat.increment();
  } else {
    _collection.insert(it, StatCollection::value_type(_key, Stat(key, size)));
  }
  _mutex.unlock();
}

// protected
void Stats::collect() {
  logger->info(CONTEXT, "Starting stats collection");
  Elem e;
  while (state.isRunning()) {
    if (barrier->consume(e)) { // got one
#ifdef _DEBUG
      logger->trace(CONTEXT,
                   "Consumed stat: %s, %d", e.first.c_str(), e.second);
#endif
      increment(e.first, e.second);
    } else { // did not get one
#ifdef _DEVEL
      // FIXME this is useless without sampling it
      logger->trace(CONTEXT, "No stat to consume");
#endif
    }
    // FIXME this should usleep (backoff style) if needed
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

} // end namespace mctop
