#ifndef _REPORT_TEXT_H
#define _REPORT_TEXT_H

#include "report/report.h"
#include "config.h"

namespace mckeys {

class TextReport : public Report
{
 public:
  TextReport(const Config* cfg, const Stats* stats);
  virtual void render();

 private:
  const Stats* stats;
};

} // end namespace

#endif
