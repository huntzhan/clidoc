#include <set>
#include <string>

#include "clidoc/cpp11.h"
#include "gtest/gtest.h"

template <typename Map>
std::set<std::string> ExtractKeys(const Map &outcome) {
  std::set<std::string> keys;
  for (const auto &map_pair : outcome) {
    keys.insert(map_pair.first);
  }
  return keys;
}

TEST(simple_option, run) {
  const char *argv[] = {"utility_name", "-a"};
  clidoc::ParseArguments(2, argv);
  std::set<std::string> expected = {"-a", "--long-1"};
  EXPECT_EQ(expected, ExtractKeys(clidoc::boolean_outcome));
}

TEST(simple_option, fail) {
  const char *argv[] = {"utility_name", "fail"};
  EXPECT_FALSE(
      clidoc::ParseArguments(
          2, argv, clidoc::SYSTEM_EXIT_OFF | clidoc::PRINT_DOC_OFF));
}
