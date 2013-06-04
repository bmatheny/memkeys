#ifndef SRC_CLI_H_
#define SRC_CLI_H_

/**
 * Copyright 2013 Blake Matheny
 */
#include <string>

namespace mckeys {

class Config;

class Cli {
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

}  // namespace mckeys
#endif  // SRC_CLI_H_
