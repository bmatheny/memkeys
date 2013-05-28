#include "logging/level.h"
#include "util/options.h"
#include <string>
#include <stdexcept>
#include <gtest/gtest.h>

namespace mckeys {

TEST(Level, Names) {
  ASSERT_EQ("TRACE", Level::TRACE.getName());
  ASSERT_EQ("DEBUG", Level::DEBUG.getName());
  ASSERT_EQ("INFO", Level::INFO.getName());
  ASSERT_EQ("WARNING", Level::WARNING.getName());
  ASSERT_EQ("ERROR", Level::ERROR.getName());
  ASSERT_EQ("FATAL", Level::FATAL.getName());
  ASSERT_EQ("OFF", Level::OFF.getName());
}

#define NAME_TEST(lvl, ucn, id) do { \
  ASSERT_EQ(lvl, Level::fromName(ucn)); \
  ASSERT_EQ(lvl, Level::fromValue(id)); \
} while(0)
TEST(Level, NameMap) {
  NAME_TEST(Level::TRACE, "TRACE", 0);
  NAME_TEST(Level::DEBUG, "DEBUG", 1);
  NAME_TEST(Level::INFO, "INFO", 2);
  NAME_TEST(Level::WARNING, "WARNING", 3);
  NAME_TEST(Level::ERROR, "ERROR", 4);
  NAME_TEST(Level::FATAL, "FATAL", 5);
  NAME_TEST(Level::OFF, "OFF", USIZE_BITS(uint32_t));
  ASSERT_THROW(Level::fromName("trace"), std::range_error);
  ASSERT_THROW(Level::fromValue(1942), std::range_error);
}

TEST(Level, Equality) {
  ASSERT_LT(Level::TRACE, Level::DEBUG);
  ASSERT_LT(Level::DEBUG, Level::INFO);
  ASSERT_LT(Level::INFO, Level::WARNING);
  ASSERT_LT(Level::WARNING, Level::ERROR);
  ASSERT_LT(Level::ERROR, Level::FATAL);
  ASSERT_LT(Level::FATAL, Level::OFF);

  ASSERT_GT(Level::OFF, Level::FATAL);
  ASSERT_GT(Level::FATAL, Level::ERROR);
  ASSERT_GT(Level::ERROR, Level::WARNING);
  ASSERT_GT(Level::WARNING, Level::INFO);
  ASSERT_GT(Level::INFO, Level::DEBUG);
  ASSERT_GT(Level::DEBUG, Level::TRACE);

  ASSERT_EQ(Level::OFF, Level::OFF);
  ASSERT_EQ(Level::FATAL, Level::FATAL);
  ASSERT_EQ(Level::ERROR, Level::ERROR);
  ASSERT_EQ(Level::WARNING, Level::WARNING);
  ASSERT_EQ(Level::INFO, Level::INFO);
  ASSERT_EQ(Level::DEBUG, Level::DEBUG);
  ASSERT_EQ(Level::TRACE, Level::TRACE);

  ASSERT_NE(Level::OFF, Level::FATAL);
  ASSERT_NE(Level::OFF, Level::ERROR);
  ASSERT_NE(Level::OFF, Level::WARNING);
  ASSERT_NE(Level::OFF, Level::INFO);
  ASSERT_NE(Level::OFF, Level::DEBUG);
  ASSERT_NE(Level::OFF, Level::TRACE);
}

} // end namespace
