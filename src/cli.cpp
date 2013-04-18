#include <iostream>
#include <sstream>
#include <cstdlib>

extern "C" {
  #include <getopt.h>
}

#include "cli.h"
#include "config.h"
#include "options.h"

#ifdef HAVE_CONFIG_H
#include "mconfig.h"
#endif

#ifndef PACKAGE_STRING
#define PACKAGE_STRING "mctop (unknown version)"
#endif
#ifndef PACKAGE_BUGREPORT
#define PACKAGE_BUGREPORT "bmatheny@mobocracy.net"
#endif

namespace mctop {

static const struct option longopts[] = {
  {"discard", required_argument, 0, 'd'},
  {"interface", required_argument, 0, 'i'},
  {"port", required_argument, 0, 'p'},
  {"refresh", required_argument, 0, 'r'},
  {"help", no_argument, 0, 'h'},
  {"verbose", no_argument, 0, 'v'},
  {"version", no_argument, 0, 'V'},
};
static const char * argp = "d:i:p:r:hvV";

using namespace std;

string Cli::help(const char * progname)
{
  ostringstream txt;
  string pname = progname;
  string tab = "    ";
  txt << "Usage: " << pname << " -i NIC [options]" << endl;
  txt << mkHelpDoc(longopts[0], "Discard keys where req/s rate is below THRESH", "THRESH");
  txt << mkHelpDoc(longopts[1], "Network interface to capture traffic on (required)", "NIC");
  txt << mkHelpDoc(longopts[2], "Network port to capture memcache traffic on (default 11211)", "PORT");
  txt << mkHelpDoc(longopts[3], "Refresh the stats display every INTERVAL ms (default 500)", "INTERVAL");
  txt << endl;
  txt << mkHelpDoc(longopts[4], "This help", "");
  txt << mkHelpDoc(longopts[5], "Increase verbosity. May be used multiple times.", "");
  txt << mkHelpDoc(longopts[6], "Show program info and exit.", "");
  return txt.str();
}

/**
 * Parse the command line arguments, updating a config as appropriate.
 */
void Cli::parse(int argc, char ** argv, Config * cfg)
{
  int c;
  char * progname = argv[0];
  while (1) {
    int option_index = 0;
    c = getopt_long(argc, argv, argp, longopts, &option_index);
    if (c == -1)
      break;
    switch (c) {
      case 'd':
        cfg->setDiscardThreshold(::atof(optarg));
        break;
      case 'i':
        cfg->setInterface(optarg);
        break;
      case 'p':
        cfg->setPort(::atoi(optarg));
        break;
      case 'r':
        cfg->setRefreshInterval(::atoi(optarg));
        break;
      case 'h':
        cout << Cli::help(progname);
        exit(EXIT_SUCCESS);
      case 'v':
        cfg->increaseVerbosity();
        break;
      case 'V':
        cout << "Package: " << PACKAGE_STRING << endl;
        cout << "Author : " << PACKAGE_BUGREPORT << endl;
        cout << "License: Apache 2.0" << endl;
        exit(EXIT_SUCCESS);
      case '?':
        cout << "Unknown flag specified" << endl;
        cout << Cli::help(progname);
        exit(EXIT_FAILURE);
      default:
        cout << "Unknown flag specified" << endl;
        cout << Cli::help(progname);
        exit(EXIT_FAILURE);
    }
  }
}

string Cli::mkHelpLead(const struct option opt, const string &key)
{
  ostringstream txt;
  ssize_t len = 0, alloc = 33;
  char * os = NULL;
  txt << "    -" << ((char)opt.val) << ", --" << opt.name;
  if (opt.has_arg == 1) {
    txt << "=" << key;
  } else if (opt.has_arg == 2) {
    txt << "[=" << key << "]";
  }
  len = txt.str().length();
  alloc += len;
  os = (char*)malloc(sizeof(char)*alloc);
  snprintf(os, alloc, "%-32s", txt.str().c_str());
  return string(os);
}

string Cli::mkHelpDoc(const struct option opt, const string &desc, const string &key)
{
  ostringstream txt;
  string lead = mkHelpLead(opt, key);
  txt << lead << desc << endl;
  return txt.str();
}

} // end namespace
