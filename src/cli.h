#ifndef CLI_H
#define CLI_H

#include "config.h"

namespace mckeys {

class Cli
{
 public:
  static void parse(int argc, char ** argv, Config * cfg);
  static std::string help(const char * progname);

 private:
  static std::string mkHelpLead(const struct option opt,
                                const std::string &key);
  static std::string mkHelpDoc(const struct option opt,
                               const std::string &desc,
                               const std::string &key);
};

} // end namespace
#endif
