#include "logging/record.h"
#include "logging/level.h"
#include <string>
#include <gtest/gtest.h>

namespace mckeys {

TEST(Record, DefaultConstructor) {
  Record rec;
  EXPECT_TRUE(rec.getFileName().empty());
  EXPECT_EQ(Level::OFF, rec.getLevel());
}

TEST(Record, FileName) {
  Record rec;
  rec.setFileName("fizz.txt");
  EXPECT_EQ("fizz.txt", rec.getFileName());
}

TEST(Record, Level) {
  Record rec;
  rec.setLevel(Level::DEBUG);
  EXPECT_EQ(Level::DEBUG, rec.getLevel());
  rec.setLevel(Level::INFO);
  EXPECT_EQ(Level::INFO, rec.getLevel());
}

TEST(Record, Timestamp) {
  Record rec1;
  struct timeval tv = {1369615982, 123000};
  auto actual = rec1.getTimestamp(tv);
  std::string expected("20130527-00:53:02.123");
  EXPECT_EQ(expected, actual);
}

} // end namespace
