#include "gtest/gtest.h"
#include "clidoc/cpp11.h"
#include "cpp11_codegen_test/util-inl.h"

const set<string> kBooleanKeys = {
  "command",
  "what-ever",
};
const set<string> kStringKeys = {};
const set<string> kStringListKeys = {};
auto KeyChecker = GenerateKeysOfOutcomeChecker(
    kBooleanKeys, kStringKeys, kStringListKeys);

TEST(command, run) {
  const char *argv_1[] = {"utility_name", "command"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv_1, kTestMode));
  KeyChecker();
  EXPECT_TRUE(
      clidoc::boolean_outcome["command"]);

  const char *argv_2[] = {"utility_name", "what-ever"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv_2, kTestMode));
  KeyChecker();
  EXPECT_TRUE(
      clidoc::boolean_outcome["what-ever"]);
}

TEST(command, fail) {
  const char *argv_1[] = {"utility_name", "COMMAND"};
  EXPECT_FALSE(
      clidoc::ParseArguments(2, argv_1, kTestMode));

  const char *argv_2[] = {"utility_name", "command", "something else"};
  EXPECT_FALSE(
      clidoc::ParseArguments(3, argv_2, kTestMode));
}
