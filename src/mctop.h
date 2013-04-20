#ifndef _MCTOP_MCTOP_H
#define _MCTOP_MCTOP_H

#include <string>

#include "common.h"
#include "net/net.h"
#include "report/report.h"

namespace mctop {

class Mctop {
 public:
  static Mctop * getInstance(const Config * config);
  static Mctop * getInstance(int argc, char ** argv);
  virtual ~Mctop();

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
  Mctop(const Config * config);

  const Config * config;
  const LoggerPtr logger;
  Pcap * session;
  CaptureEngine * engine;
  State state;
};

} // end namespace mctop

#endif
