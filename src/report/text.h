#ifndef _REPORT_TEXT_H
#define _REPORT_TEXT_H

#include "report/report.h"
#include "config.h"

namespace mctop {

class TextReport : public Report
{
 public:
  TextReport(const Config * cfg);
  virtual void render();
};

} // end namespace mctop

#endif
