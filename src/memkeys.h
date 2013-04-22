#ifndef _MEMKEYS_MEMKEYS_H
#define _MEMKEYS_MEMKEYS_H

#include <string>

#include "common.h"
#include "net/net.h"
#include "report/report.h"

namespace mckeys {

class Memkeys {
 public:
  static Memkeys * getInstance(const Config * config);
  static Memkeys * getInstance(int argc, char ** argv);
  virtual ~Memkeys();

  virtual void run();

  // called by signal handler
  void tryShutdown();
  // called by signal handler after timeout
  void forceShutdown();

  bool isRunning() const
    { return state.isRunning(); }
  bool isShutdown() const
    { return state.isTerminated(); }
  std::string getStateName() const
    { return state.getName(); }

 protected:
  Memkeys(const Config * config);

  const Config * config;
  const LoggerPtr logger;
  Pcap * session;
  CaptureEngine * engine;
  State state;
};

} // end namespace mctop

#endif
