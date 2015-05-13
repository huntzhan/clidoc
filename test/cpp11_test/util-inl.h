#include <functional>
#include <set>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "clidoc/cpp11.h"

using std::set;
using std::string;
using std::vector;

const auto kTestMode = clidoc::SYSTEM_EXIT_OFF | clidoc::PRINT_DOC_OFF;

template <typename Map>
std::set<std::string> ExtractKeys(const Map &outcome) {
  std::set<std::string> keys;
  for (const auto &map_pair : outcome) {
    keys.insert(map_pair.first);
  }
  return keys;
}

std::function<void ()> GenerateKeysOfOutcomeChecker(
    const std::set<std::string> &boolean_keys,
    const std::set<std::string> &string_keys,
    const std::set<std::string> &string_list_keys) {
  return [&]() {
    EXPECT_EQ(
        boolean_keys,
        ExtractKeys(clidoc::boolean_outcome));
    EXPECT_EQ(
        string_keys,
        ExtractKeys(clidoc::string_outcome));
    EXPECT_EQ(
        string_list_keys,
        ExtractKeys(clidoc::string_list_outcome));
  };
}
