#ifndef _REPORT_TEXT_H
#define _REPORT_TEXT_H

#include "config.h"
#include "report/report.h"
#include "util/stats.h"

namespace mckeys {

class TextReport : public Report
{
 public:
  TextReport(const Config* cfg, Stats* stats);
  virtual void render();

 protected:
  void renderStats(std::deque<Stat> q);

 private:
  Stats* stats;
};

} // end namespace

#endif
