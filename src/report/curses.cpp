#include <iomanip>
#include <iterator>

#include "common.h"
#include "report/curses.h"

extern "C" {
#include <signal.h>
}

namespace mckeys {

using namespace std;

CursesReport::CursesReport(const Config* cfg, const Pcap* session, Stats* stats)
  : Report(cfg, Logger::getLogger("cursesReport")),
    session(session),
    stats(stats),
    statColumnWidth(10),
    keyColumnWidth(0),
    sortOrder(sort_DESC),
    sortMode(mode_REQRATE)
{
  vector<string> vec = {"calls", "objsize", "req/sec", "bw(kbps)"};
  statColumns.reserve(vec.size());
  for (uint32_t i = 0; i < vec.size(); i++) {
    ostringstream oss;
    oss << setw(statColumnWidth) << vec.at(i);
    statColumns.push_back(oss.str());
  }
  cmdMap = {
    {'B', "sort by bandwidth"},
    {'C', "sort by calls"},
    {'Q', "quit"},
    {'R', "sort by req/sec"},
    {'S', "sort by size"},
    {'T', "toggle sort order (asc|desc)"}
  };
  if (state.checkAndSet(state_NEW, state_STARTING)) {
    report_thread = thread(&CursesReport::render, this);
  } else {
    logger->warning(CONTEXT, "Incorrect API usage");
  }
}

// Tear down curses interface
CursesReport::~CursesReport() {
  renderDone();
}

void CursesReport::render()
{
  uint64_t render_start_time = 0, render_duration = 0;
  struct timespec ts = UtilTime::millisToTimespec(config->getRefreshInterval());
  string render_string;

  if (!state.checkAndSet(state_STARTING, state_RUNNING)) {
    logger->error(CONTEXT, "render already started");
    return;
  }
  renderInit();

  while(state.isRunning()) {
    render_start_time = UtilTime::currentTimeMillis();
    renderHeader();
    renderFooter();
    deque<Stat> q = stats->getLeaders(sortMode, sortOrder);
    uint32_t qsize = q.size();
    logger->debug(CONTEXT, "Rendering report with %u data points", qsize);
    if (qsize > 0) {
      renderStats(q, qsize);
    }
    render_duration = UtilTime::currentTimeMillis() - render_start_time;
    renderTimeStat(render_duration);
    refresh();
    char c = getch();
    handleKeyPress(c);
    nanosleep(&ts, NULL);
  }
}

///////////////////////////////////////////////////////////////////////////////
//                      Protected Methods                                    //
///////////////////////////////////////////////////////////////////////////////
void CursesReport::setpos(int y, int x) const {
  move(y, x);
}

void CursesReport::renderHeader() {
  ostringstream header;
  uint32_t colSize = statColumns.size();

  keyColumnWidth = COLS - (colSize * statColumnWidth);
  attrset(COLOR_PAIR(1));
  setpos(0, 0);

  header << left << setw(keyColumnWidth) << "memcache key";
  for (uint32_t i = 0; i < statColumns.size(); i++) {
    header << statColumns.at(i);
  }
  addstr(header.str().c_str());
}

void CursesReport::renderFooter() {
  ostringstream footer;
  setpos(LINES - 1, 0);
  attrset(COLOR_PAIR(2));
  footer << left << setw(COLS) << footerText;
  addstr(footer.str().c_str());
}

void CursesReport::renderTimeStat(const uint64_t render_time) const {
  attrset(COLOR_PAIR(2));
  setpos(LINES-2, COLS-18);
  addstr(createRenderTime(render_time).c_str());
}

void CursesReport::renderStats(deque<Stat> q, uint32_t qsize) {
  uint32_t i = 0;
  uint32_t maxlines = LINES - 3 - 1;

  string pstats = session->getStatsString();
  ostringstream summary;
  ostringstream fwSummary;

  setpos(LINES-2, 0);
  attrset(COLOR_PAIR(2));

  summary << left << setw(28) << "sort mode: reqsec (desc)";
  summary << "keys: " << setw(14) << stats->getStatCount();
  summary << setw(40) << pstats;

  fwSummary << left << setw(COLS) << summary.str();

  addstr(fwSummary.str().c_str());
  attrset(COLOR_PAIR(0));

  for (deque<Stat>::iterator it = q.begin(); it != q.end() && i < maxlines; ++it, ++i) {
    string line;
    if (i < qsize) {
      Stat stat = *it;
      line = createStatLine(stat);
    } else {
      line.assign(COLS, ' ');
    }
    setpos(i+1, 0);
    addstr(line.c_str());
  }
  for (; i < maxlines; i++) {
    string line;
    line.assign(COLS, ' ');
    setpos(i+1, 0);
    addstr(line.c_str());
  }
}

string CursesReport::createRenderTime(const uint64_t rtime) const {
  ostringstream tmp;
  tmp << "rt: " << setw(8) << fixed << setprecision(3) << rtime << " (ms)";
  return tmp.str();
}

string CursesReport::createStatLine(const Stat& stat) const {
  ostringstream tmpStat;

  // handle key
  string displayKey = stat.getKey();
  if (displayKey.size() > keyColumnWidth) {
    displayKey = displayKey.substr(0, keyColumnWidth - 4) + "...";
  }
  tmpStat << left << setw(keyColumnWidth) << displayKey;

  // handle int values
  vector<uint64_t> ints = {stat.getCount(), stat.getSize()};
  for (uint32_t i = 0; i < ints.size(); i++) {
    ostringstream dtmp;
    dtmp << setw(statColumnWidth) << ints.at(i);
    tmpStat << dtmp.str();
  }

  // handle double values
  vector<double> doubles = {stat.requestRate(), stat.bandwidth()};
  for (uint32_t i = 0; i < doubles.size(); i++) {
    ostringstream dtmp;
    dtmp << setw(statColumnWidth) << fixed << setprecision(2) << doubles.at(i);
    tmpStat << dtmp.str();
  }
  return tmpStat.str();
}

void CursesReport::handleKeyPress(char key) {
  switch(key) {
    case 'B':
    case 'b':
      sortMode = mode_BANDWIDTH;
      break;
    case 'C':
    case 'c':
      sortMode = mode_CALLS;
      break;
    case 'Q':
    case 'q':
      raise(SIGINT);
      break;
    case 'R':
    case 'r':
      sortMode = mode_REQRATE;
      break;
    case 'S':
    case 's':
      sortMode = mode_SIZE;
      break;
    case 'T':
    case 't':
      if (sortOrder == sort_DESC) {
        sortOrder = sort_ASC;
      } else {
        sortOrder = sort_DESC;
      }
      break;
    default:
      break;
  }
}

///////////////////////////////////////////////////////////////////////////////
//                      Private Methods                                      //
///////////////////////////////////////////////////////////////////////////////
void CursesReport::initFooterText() {
  ostringstream footer;
  vector<string> tmp;

  // dump command map into tmp as key:value pairs
  for (CommandMap::iterator it = cmdMap.begin(); it != cmdMap.end(); ++it)
  {
    string stmp;
    stmp += it->first;
    stmp += ":";
    stmp += it->second;
    tmp.push_back(stmp);
  }
  // use ostream_iterator to join each elem in tmp with a pipe
  copy(tmp.begin(), tmp.end() - 1, ostream_iterator<string>(footer, " | "));
  // we left off the last elem so append it
  footer << *tmp.rbegin();
  footerText = footer.str();
}

void CursesReport::renderInit() {
  initscr();
  cbreak();
  curs_set(0);
  timeout(0);
  if (can_change_color()) {
    start_color();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_RED);
  }
  initFooterText();
}

void CursesReport::renderDone() {
  nocbreak();
  endwin();
}

} // end namespace
