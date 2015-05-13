#include "gtest/gtest.h"
#include "clidoc/cpp11.h"
#include "cpp11_test/util-inl.h"

const set<string> kBooleanKeys = {
  "-a", "-b", "-c",
  "-d", "-e", "-f",
  "-g",
  "-h", "flag_h",
  "-i", "-j",
  "-k", "-l", "-m", "-n",
  "-o", "-p", "-q", "-r", "flag_opqr",
};
const set<string> kStringKeys = {};
const set<string> kStringListKeys = {};
auto KeyChecker = GenerateKeysOfOutcomeChecker(
    kBooleanKeys, kStringKeys, kStringListKeys);

TEST(logic, abc) {
  const char *argv_1[] = {"utility_name", "-a", "-b", "-c"};
  EXPECT_TRUE(
      clidoc::ParseArguments(4, argv_1, kTestMode));
  KeyChecker();
  EXPECT_TRUE(
      clidoc::boolean_outcome["-a"]);
  EXPECT_TRUE(
      clidoc::boolean_outcome["-b"]);
  EXPECT_TRUE(
      clidoc::boolean_outcome["-c"]);

  const char *argv_2[] = {"utility_name", "-abc"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv_2, kTestMode));
  KeyChecker();
  EXPECT_TRUE(
      clidoc::boolean_outcome["-a"]);
  EXPECT_TRUE(
      clidoc::boolean_outcome["-b"]);
  EXPECT_TRUE(
      clidoc::boolean_outcome["-c"]);
}

TEST(logic, def) {
  const char *argv_1[] = {"utility_name", "-d", "-e", "-f"};
  EXPECT_TRUE(
      clidoc::ParseArguments(4, argv_1, kTestMode));
  KeyChecker();
  EXPECT_TRUE(
      clidoc::boolean_outcome["-d"]);
  EXPECT_TRUE(
      clidoc::boolean_outcome["-e"]);
  EXPECT_TRUE(
      clidoc::boolean_outcome["-f"]);

  const char *argv_2[] = {"utility_name", "-fed"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv_2, kTestMode));
  KeyChecker();
  EXPECT_TRUE(
      clidoc::boolean_outcome["-d"]);
  EXPECT_TRUE(
      clidoc::boolean_outcome["-e"]);
  EXPECT_TRUE(
      clidoc::boolean_outcome["-f"]);
}

TEST(logic, g) {
  const char *argv_1[] = {"utility_name", "-g"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv_1, kTestMode));
  KeyChecker();
  EXPECT_TRUE(
      clidoc::boolean_outcome["-g"]);
}

TEST(logic, h) {
  const char *argv_1[] = {"utility_name", "flag_h"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv_1, kTestMode));
  KeyChecker();
  EXPECT_FALSE(
      clidoc::boolean_outcome["-h"]);
}

TEST(logic, ij) {
  const char *argv_1[] = {"utility_name", "-i"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv_1, kTestMode));
  KeyChecker();
  EXPECT_TRUE(
      clidoc::boolean_outcome["-i"]);

  const char *argv_2[] = {"utility_name", "-ij"};
  EXPECT_FALSE(
      clidoc::ParseArguments(2, argv_2, kTestMode));
}

TEST(logic, klmn) {
  const char *argv_1[] = {"utility_name", "-klmn"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv_1, kTestMode));
  KeyChecker();
  EXPECT_TRUE(
      clidoc::boolean_outcome["-k"]);
  EXPECT_TRUE(
      clidoc::boolean_outcome["-l"]);
  EXPECT_TRUE(
      clidoc::boolean_outcome["-m"]);
  EXPECT_TRUE(
      clidoc::boolean_outcome["-n"]);

  const char *argv_2[] = {"utility_name", "-kmn"};
  EXPECT_FALSE(
      clidoc::ParseArguments(2, argv_2, kTestMode));

  const char *argv_3[] = {"utility_name", "-lmn"};
  EXPECT_FALSE(
      clidoc::ParseArguments(2, argv_3, kTestMode));
}

TEST(logic, opqr) {
  const char *argv_1[] = {"utility_name", "-opqr", "flag_opqr"};
  EXPECT_TRUE(
      clidoc::ParseArguments(3, argv_1, kTestMode));
  KeyChecker();
  EXPECT_TRUE(
      clidoc::boolean_outcome["-o"]);
  EXPECT_TRUE(
      clidoc::boolean_outcome["-p"]);
  EXPECT_TRUE(
      clidoc::boolean_outcome["-q"]);
  EXPECT_TRUE(
      clidoc::boolean_outcome["-r"]);

  const char *argv_2[] = {"utility_name", "-or", "flag_opqr"};
  EXPECT_TRUE(
      clidoc::ParseArguments(3, argv_2, kTestMode));
  KeyChecker();
  EXPECT_TRUE(
      clidoc::boolean_outcome["-o"]);
  EXPECT_FALSE(
      clidoc::boolean_outcome["-p"]);
  EXPECT_FALSE(
      clidoc::boolean_outcome["-q"]);
  EXPECT_TRUE(
      clidoc::boolean_outcome["-r"]);

  const char *argv_3[] = {"utility_name", "flag_opqr"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv_3, kTestMode));
  KeyChecker();
  EXPECT_FALSE(
      clidoc::boolean_outcome["-o"]);
  EXPECT_FALSE(
      clidoc::boolean_outcome["-p"]);
  EXPECT_FALSE(
      clidoc::boolean_outcome["-q"]);
  EXPECT_FALSE(
      clidoc::boolean_outcome["-r"]);

  const char *argv_4[] = {"utility_name", "-pq", "flag_opqr"};
  EXPECT_FALSE(
      clidoc::ParseArguments(3, argv_4, kTestMode));

  const char *argv_5[] = {"utility_name", "-opr", "flag_opqr"};
  EXPECT_FALSE(
      clidoc::ParseArguments(3, argv_5, kTestMode));
}
