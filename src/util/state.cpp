#include "util/state.h"
#include <mutex>

namespace mctop {

using namespace std;

State::State()
    : _state(state_NEW),
      _mutex()
{}

State::State(state_t state)
    : _state(state),
      _mutex()
{}
State::~State()
{}

bool State::checkAndSet(const state_t expected, const state_t next)
{
  _mutex.lock();
  state_t state = _state;
  if (state == expected) {
    _state = next;
  }
  _mutex.unlock();
  return (state == expected);
}

state_t State::getState() const
{
  _mutex.lock();
  state_t state = _state;
  _mutex.unlock();
  return state;
}
void State::setState(const state_t state)
{
  _mutex.lock();
  _state = state;
  _mutex.unlock();
}
string State::getName() const
{
  switch (_state) {
    case state_NEW:
      return "NEW";
    case state_STARTING:
      return "STARTING";
    case state_RUNNING:
      return "RUNNING";
    case state_STOPPING:
      return "STOPPING";
    case state_TERMINATED:
      return "TERMINATED";
    default:
      return "UNKNOWN";
  }
}

} // end namespace mctop
