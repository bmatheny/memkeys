#ifndef _UTIL_STATE_H
#define _UTIL_STATE_H

#include <mutex>
#include <string>

namespace mctop {

enum state_t {
  state_NEW, state_STARTING, state_RUNNING, state_STOPPING, state_TERMINATED
};

class State;

// FIXME - profiling shows that we may want to use a std::atomic instead of a
// mutex

// thread safe state representation
class State
{
 public:
  State();
  State(state_t state);
  virtual ~State();

  bool checkAndSet(const state_t expected, const state_t next);
  state_t getState() const;
  std::string getName() const;
  void setState(const state_t state);
  bool isNew() const
    { return getState() == state_NEW; }
  bool isStarting() const
    { return getState() == state_STARTING; }
  bool isRunning() const
    { return getState() == state_RUNNING; }
  bool isStopping() const
    { return getState() == state_STOPPING; }
  bool isTerminated() const
    { return getState() == state_TERMINATED; }

 protected:
  state_t _state;
  mutable std::mutex _mutex;

 private:
  State(const State &state); // no copy constructor
};

} // end namespace mctop

#endif
