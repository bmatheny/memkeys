#include "logging/level.h"
#include <string>
#include <gtest/gtest.h>

namespace mckeys {

TEST(Level, Equality) {
  ASSERT_EQ("TRACE", Level::TRACE.getName());
  ASSERT_EQ("DEBUG", Level::DEBUG.getName());
  ASSERT_EQ("INFO", Level::INFO.getName());
  ASSERT_EQ("WARNING", Level::WARNING.getName());
  ASSERT_EQ("ERROR", Level::ERROR.getName());
  ASSERT_EQ("FATAL", Level::FATAL.getName());
  ASSERT_EQ("OFF", Level::OFF.getName());
}

} // end namespace
