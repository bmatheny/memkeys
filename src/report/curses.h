#ifndef _REPORT_CURSES_H
#define _REPORT_CURSES_H

#include "config.h"
#include "net/pcap.h"
#include "report/report.h"
#include "util/stats.h"

#include <map>
#include <string>
#include <vector>
#include <ncurses.h>

namespace mckeys {

class CursesReport : public Report
{
 public:
  typedef std::vector<std::string> StatColumns;
  typedef std::map<char, std::string> CommandMap;

  CursesReport(const Config* cfg, const Pcap* session, Stats* stats);
  virtual ~CursesReport();

  virtual void render();

 protected:
  void handleKeyPress(char key);
  void renderHeader();
  void renderFooter();
  void renderTimeStat(const uint64_t render_time) const;
  void renderStats(std::deque<Stat> q, uint32_t qsize);
  std::string createStatLine(const Stat &stat) const;
  std::string createRenderTime(const uint64_t render_time) const;
  void setpos(int y, int x) const;

 private:
  void renderInit(); // initialize rendering system - only once before first rendering
  void renderDone(); // tear down render system
  void initFooterText(); // done once by renderInit
  const Pcap* session;
  Stats* stats;
  int statColumnWidth;
  uint32_t keyColumnWidth;
  std::string footerText;
  StatColumns statColumns;
  CommandMap cmdMap;
  SortOrder sortOrder;
  SortMode sortMode;
};

} // end namespace

#endif
