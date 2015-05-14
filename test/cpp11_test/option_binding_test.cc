#include "gtest/gtest.h"
#include "clidoc/cpp11.h"
#include "cpp11_test/util-inl.h"

const set<string> kBooleanKeys = {
  "-c",
  "--long-4",
};
const set<string> kStringKeys = {
  "-a",
  "-b",
  "--long-1",
  "--long-2",
  "<p3>",
};
const set<string> kStringListKeys = {
  "-d",
  "-e",
  "--long-3",
};
auto KeyChecker = GenerateKeysOfOutcomeChecker(
    kBooleanKeys, kStringKeys, kStringListKeys);

TEST(option_binding, option_a) {
  const char *argv_1[] = {"utility_name", "-a", "value"};
  EXPECT_TRUE(
      clidoc::ParseArguments(3, argv_1, kTestMode));
  KeyChecker();
  EXPECT_EQ("value", clidoc::string_outcome["-a"]);

  const char *argv_2[] = {"utility_name", "-avalue"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv_2, kTestMode));
  KeyChecker();
  EXPECT_EQ("value", clidoc::string_outcome["-a"]);
}

TEST(option_binding, option_b) {
  const char *argv_1[] = {"utility_name", "-b", "value"};
  EXPECT_TRUE(
      clidoc::ParseArguments(3, argv_1, kTestMode));
  KeyChecker();
  EXPECT_EQ("value", clidoc::string_outcome["-b"]);
}

TEST(option_binding, option_c_p3) {
  const char *argv_1[] = {"utility_name", "-c", "value"};
  EXPECT_TRUE(
      clidoc::ParseArguments(3, argv_1, kTestMode));
  KeyChecker();
  EXPECT_TRUE(
      clidoc::boolean_outcome["-c"]);
  EXPECT_EQ("value", clidoc::string_outcome["<p3>"]);
}

TEST(option_binding, option_e) {
  const char *argv_1[] = {"utility_name", "-e", "a", "b", "c"};
  EXPECT_TRUE(
      clidoc::ParseArguments(5, argv_1, kTestMode));
  KeyChecker();
  vector<string> expected = {"a", "b", "c"};
  EXPECT_EQ(expected, clidoc::string_list_outcome["-e"]);
}

TEST(option_binding, guideline_8_normal) {
  const char *argv_1[] = {"utility_name", "-e", "a,b,c"};
  EXPECT_TRUE(
      clidoc::ParseArguments(3, argv_1, kTestMode));
  KeyChecker();
  vector<string> expected = {"a", "b", "c"};
  EXPECT_EQ(expected, clidoc::string_list_outcome["-e"]);
}

TEST(option_binding, guideline_8_fault_tolerance) {
  const char *argv_1[] = {"utility_name", "-e", "a,,c"};
  EXPECT_TRUE(
      clidoc::ParseArguments(3, argv_1, kTestMode));
  KeyChecker();
  vector<string> expected_1 = {"a", "c"};
  EXPECT_EQ(expected_1, clidoc::string_list_outcome["-e"]);

  const char *argv_2[] = {"utility_name", "-e", "a,c,"};
  EXPECT_TRUE(
      clidoc::ParseArguments(3, argv_2, kTestMode));
  KeyChecker();
  vector<string> expected_2 = {"a", "c"};
  EXPECT_EQ(expected_2, clidoc::string_list_outcome["-e"]);

  const char *argv_3[] = {"utility_name", "-e", ",,"};
  EXPECT_TRUE(
      clidoc::ParseArguments(3, argv_3, kTestMode));
  KeyChecker();
  vector<string> expected_3 = {};
  EXPECT_EQ(expected_3, clidoc::string_list_outcome["-e"]);
}

TEST(option_binding, option_long_1) {
  const char *argv_1[] = {"utility_name", "--long-1", "!@#^&$!"};
  EXPECT_TRUE(
      clidoc::ParseArguments(3, argv_1, kTestMode));
  KeyChecker();
  EXPECT_EQ("!@#^&$!", clidoc::string_outcome["--long-1"]);
}

TEST(option_binding, option_long_2) {
  const char *argv_1[] = {"utility_name", "--long-2", "!@#^&$!"};
  EXPECT_TRUE(
      clidoc::ParseArguments(3, argv_1, kTestMode));
  KeyChecker();
  EXPECT_EQ("!@#^&$!", clidoc::string_outcome["--long-2"]);
}

TEST(option_binding, option_long_3) {
  const char *argv_1[] = {"utility_name", "--long-3", "a", "b", "c"};
  EXPECT_TRUE(
      clidoc::ParseArguments(5, argv_1, kTestMode));
  KeyChecker();
  vector<string> expected = {"a", "b", "c"};
  EXPECT_EQ(expected, clidoc::string_list_outcome["--long-3"]);
}

TEST(option_binding, option_f_option_long_4) {
  const char *argv_1[] = {"utility_name", "-f"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv_1, kTestMode));
  KeyChecker();
  EXPECT_TRUE(clidoc::boolean_outcome["--long-4"]);

  const char *argv_2[] = {"utility_name", "--long-4"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv_2, kTestMode));
  KeyChecker();
  EXPECT_TRUE(clidoc::boolean_outcome["--long-4"]);
}
