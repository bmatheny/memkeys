#include "util/backoff.h"
#include "util/util_time.h"
#include <cstdlib> // rand & srand

namespace mckeys {

using namespace std;

// 1.5 which is 50% increase per back off
const double Backoff::DEFAULT_MULTIPLIER = 1.5;
// 0.5 which results in a random period ranging between 50% below and 50% above
// the retry interval
const double Backoff::DEFAULT_RANDOMIZATION_FACTOR = 0.5;

Backoff::Backoff()
  : initialIntervalMillis(Backoff::DEFAULT_INITIAL_INTERVAL_MILLIS),
    maxElapsedTimeMillis(Backoff::DEFAULT_MAX_ELAPSED_TIME_MILLIS),
    maxIntervalMillis(Backoff::DEFAULT_MAX_INTERVAL_MILLIS),
    multiplier(Backoff::DEFAULT_MULTIPLIER),
    randomizationFactor(Backoff::DEFAULT_RANDOMIZATION_FACTOR),
    currentRetryInterval(0)
{
  srand(time(NULL));
  reset();
}

Backoff::~Backoff() {
}

// The current retry interval in milliseconds
uint32_t Backoff::getCurrentIntervalMillis() const {
  return currentRetryInterval;
}

// The elapsed time in ms since the instance was created.
// Reset when reset() is called.
uint64_t Backoff::getElapsedTimeMillis() const {
  return UtilTime::currentTimeMillis() - startTime;
}

// The initial retry interval in milliseconds.
uint32_t Backoff::getInitialIntervalMillis() const {
  return initialIntervalMillis;
}
Backoff& Backoff::setInitialIntervalMillis(const uint32_t millis) {
  initialIntervalMillis = millis;
  return (*this);
}

// The maximum elapsed time in milliseconds
uint32_t Backoff::getMaxElapsedTimeMillis() const {
  return maxElapsedTimeMillis;
}
Backoff& Backoff::setMaxElapsedTimeMillis(const uint32_t millis) {
  maxElapsedTimeMillis = millis;
  return (*this);
}

// The max value of the back off period in milliseconds
uint32_t Backoff::getMaxIntervalMillis() const {
  return maxIntervalMillis;
}
Backoff& Backoff::setMaxIntervalMillis(const uint32_t millis) {
  maxIntervalMillis = millis;
  return (*this);
}

// Returns the value to multiply the current interval with for each retry
// attempt.
double Backoff::getMultiplier() const {
  return multiplier;
}
Backoff& Backoff::setMultiplier(const double mult) {
  multiplier = mult;
  return (*this);
}

// Gets the number of milliseconds to wait before retrying
uint64_t Backoff::getNextBackOffMillis() {
  if (getElapsedTimeMillis() > getMaxElapsedTimeMillis()) {
    return currentRetryInterval;
  }
  uint32_t curint = getCurrentIntervalMillis();
  double delta = getRandomizationFactor() * curint;
  double minInterval = curint - delta;
  double maxInterval = curint + delta;
  double random = (float) rand()/RAND_MAX;
  int randInterval = (int)(minInterval + (random * (maxInterval - minInterval + 1)));
  if (curint >= (getMaxIntervalMillis() / getMultiplier())) {
    currentRetryInterval = getMaxIntervalMillis();
  } else {
    currentRetryInterval *= getMultiplier();
  }
  return randInterval;
}

// Returns the randomization factor to use for creating a range around the
// retry interval
double Backoff::getRandomizationFactor() const {
  return randomizationFactor;
}
Backoff& Backoff::setRandomizationFactor(const double factor) {
  randomizationFactor = factor;
  return (*this);
}

void Backoff::reset() {
  startTime = UtilTime::currentTimeMillis();
  currentRetryInterval = getInitialIntervalMillis();
}

} // end namespace
