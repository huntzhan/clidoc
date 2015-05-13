#include "gtest/gtest.h"
#include "clidoc/cpp11.h"
#include "cpp11_test/util-inl.h"

const set<string> kBooleanKeys = {
  "-a",
  "--long-1",
};
const set<string> kStringKeys = {};
const set<string> kStringListKeys = {};
auto KeyChecker = GenerateKeysOfOutcomeChecker(
    kBooleanKeys, kStringKeys, kStringListKeys);

TEST(simple_option, option_a) {
  const char *argv[] = {"utility_name", "-a"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv, kTestMode));
  KeyChecker();
  EXPECT_TRUE(
      clidoc::boolean_outcome["-a"]);
}

TEST(simple_option, option_long_1) {
  const char *argv[] = {"utility_name", "--long-1"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv, kTestMode));
  KeyChecker();
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
