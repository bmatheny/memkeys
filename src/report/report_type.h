#ifndef _REPORT_REPORT_TYPE_H
#define _REPORT_REPORT_TYPE_H

#include <string>

namespace mckeys {

// forward declarations because shit we have some recursive dependencies
class Config;
class Pcap;
class Stats;
class Report;

class ReportType
{
 public:
  static ReportType NCURSES;
  static ReportType CSV;

  static ReportType fromString(const std::string &name);

  Report* makeReport(const Config* cfg, const Pcap* sess, Stats* stats);
  std::string getName() const;

  bool operator==(const ReportType &other) const;

 protected:
  std::string name;
  ReportType(const std::string &name);

};

} // end namespace

#endif
