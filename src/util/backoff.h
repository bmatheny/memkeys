#ifndef _UTIL_BACKOFF_H
#define _UTIL_BACKOFF_H

#include <cstdint>

namespace mckeys {

// Modeled off http://goo.gl/AfeaV
// Provides a configurable backoff policy for something that needs it
class Backoff {
 public:
  // 0.5 seconds
  const static uint32_t DEFAULT_INITIAL_INTERVAL_MILLIS = 5;
  // 15 minutes
  const static uint32_t DEFAULT_MAX_ELAPSED_TIME_MILLIS = 1000;
  // 1 minute
  const static uint32_t DEFAULT_MAX_INTERVAL_MILLIS = 1000;
  // 1.5 which is 50% increase per back off
  const static double DEFAULT_MULTIPLIER; // = 1.5;
  // 0.5 which results in a random period ranging between 50% below and 50%
  // above the retry interval
  const static double DEFAULT_RANDOMIZATION_FACTOR; // = 0.5;

  Backoff();
  virtual ~Backoff();

  // The current retry interval in milliseconds
  uint32_t getCurrentIntervalMillis() const;

  // The elapsed time in ms since the instance was created.
  // Reset when reset() is called.
  uint64_t getElapsedTimeMillis() const;

  // The initial retry interval in milliseconds.
  uint32_t getInitialIntervalMillis() const;
  Backoff& setInitialIntervalMillis(const uint32_t millis);

  // The maximum elapsed time in milliseconds
  uint32_t getMaxElapsedTimeMillis() const;
  Backoff& setMaxElapsedTimeMillis(const uint32_t millis);

  // The max value of the back off period in milliseconds
  uint32_t getMaxIntervalMillis() const;
  Backoff& setMaxIntervalMillis(const uint32_t millis);

  // Returns the value to multiply the current interval with for each retry
  // attempt.
  double getMultiplier() const;
  Backoff& setMultiplier(const double multiplier);

  // Gets the number of milliseconds to wait before retrying
  uint64_t getNextBackOffMillis();

  // Returns the randomization factor to use for creating a range around the
  // retry interval
  double getRandomizationFactor() const;
  Backoff& setRandomizationFactor(const double factor);

  void reset();

 private:
  uint32_t initialIntervalMillis;
  uint32_t maxElapsedTimeMillis;
  uint32_t maxIntervalMillis;
  double multiplier;
  double randomizationFactor;
  uint64_t startTime;
  uint32_t currentRetryInterval;
};

} // end namespace

#endif
