#include <set>
#include <string>

#include "clidoc/cpp11.h"
#include "gtest/gtest.h"

const auto kTestMode = clidoc::SYSTEM_EXIT_OFF | clidoc::PRINT_DOC_OFF;

template <typename Map>
std::set<std::string> ExtractKeys(const Map &outcome) {
  std::set<std::string> keys;
  for (const auto &map_pair : outcome) {
    keys.insert(map_pair.first);
  }
  return keys;
}

TEST(simple_option, option_a) {
  const char *argv[] = {"utility_name", "-a"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv, kTestMode));
  EXPECT_TRUE(
      clidoc::boolean_outcome["-a"]);
}

TEST(simple_option, option_long_1) {
  const char *argv[] = {"utility_name", "--long-1"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv, kTestMode));
  EXPECT_TRUE(
      clidoc::boolean_outcome["--long-1"]);
}

TEST(simple_option, fail_1) {
  const char *argv[] = {"utility_name", "--whatever"};
  EXPECT_FALSE(clidoc::ParseArguments(2, argv, kTestMode));
}


TEST(simple_option, fail_2) {
  const char *argv[] = {"utility_name", "fail"};
  EXPECT_FALSE(clidoc::ParseArguments(2, argv, kTestMode));
}


TEST(simple_option, fail_3) {
  const char *argv[] = {"utility_name", "-a", "something else"};
  EXPECT_FALSE(clidoc::ParseArguments(3, argv, kTestMode));
}
