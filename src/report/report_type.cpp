#include <stdexcept>

#include "common.h"
#include "report/report_type.h"
#include "report/csv.h"
#include "report/curses.h"

namespace mckeys {

using namespace std;

// protected constructor
ReportType::ReportType(const string &name) : name(name)
{}

// static values
ReportType ReportType::CSV = ReportType("CSV");
ReportType ReportType::NCURSES = ReportType("CURSES");

// static methods
ReportType ReportType::fromString(const string &name) {
  string reportName = "";
  for (uint32_t i = 0; i < name.length(); i++) {
    reportName += toupper(name.at(i));
  }
  if (reportName == "CURSES") {
    return ReportType::NCURSES;
  } else if (reportName == "CSV") {
    return ReportType::CSV;
  } else {
    throw range_error("No such report type with name " + name);
  }
}

// public methods
string ReportType::getName() const {
  return name;
}
bool ReportType::operator==(const ReportType &other) const {
  return (getName() == other.getName());
}

Report* ReportType::makeReport(const Config* cfg, const Pcap* sess, Stats* stats) {
  if (cfg->getReportType() == ReportType::NCURSES) {
    return new CursesReport(cfg, sess, stats);
  } else if (cfg->getReportType() == ReportType::CSV) {
    return new CsvReport(cfg, sess, stats);
  } else {
    throw range_error("Unsupported report type " + cfg->getReportType().getName());
  }
}

} // end namespace
