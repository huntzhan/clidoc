#include <string>
#include <vector>
#include <map>
#include <set>

#include "gtest/gtest.h"
#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/argv_processor.h"

using std::string;
using std::vector;
using std::map;
using std::set;
using namespace clidoc;

TEST(ArgvProcessorTest, DetectArgumentPattern) {
  ArgvProcessLogic logic({}, {}, {});
  EXPECT_EQ(
      ArgvProcessLogic::ArgumentPattern::POSIX_OPTION,
      logic.DetectArgumentPattern("-c"));
  EXPECT_EQ(
      ArgvProcessLogic::ArgumentPattern::GNU_OPTION,
      logic.DetectArgumentPattern("--long-option"));

  EXPECT_EQ(
      ArgvProcessLogic::ArgumentPattern::SINGLE_DASH_CASE,
      logic.DetectArgumentPattern("-abcDEF"));
  EXPECT_EQ(
      ArgvProcessLogic::ArgumentPattern::SINGLE_DASH_CASE,
      logic.DetectArgumentPattern("-abcDEF()%@*%()"));

  EXPECT_EQ(
      ArgvProcessLogic::ArgumentPattern::DOUBLE_DASH_CASE,
      logic.DetectArgumentPattern("--long-option=something"));
  EXPECT_EQ(
      ArgvProcessLogic::ArgumentPattern::DOUBLE_DASH_CASE,
      logic.DetectArgumentPattern("--abcDEF()%@*%()"));

  EXPECT_EQ(
      ArgvProcessLogic::ArgumentPattern::UNKNOW_CASE,
      logic.DetectArgumentPattern("command"));
  EXPECT_EQ(
      ArgvProcessLogic::ArgumentPattern::UNKNOW_CASE,
      logic.DetectArgumentPattern("--"));
}

TEST(ArgvProcessorTest, ReplaceWithRepresentativeOption) {
  map<Token, Token> option_to_rep_option = {
    {Token(TerminalType::POSIX_OPTION, "-c"),
     Token(TerminalType::GNU_OPTION, "--longc")},
    {Token(TerminalType::GNU_OPTION, "--longc"),
     Token(TerminalType::GNU_OPTION, "--longc")},
  };
  ArgvProcessLogic logic({}, option_to_rep_option, {});

  auto c = Token(TerminalType::POSIX_OPTION, "-c");
  auto longc = Token(TerminalType::GNU_OPTION, "--longc");
  auto expectedc = Token(TerminalType::GNU_OPTION, "--longc");
  EXPECT_TRUE(
      logic.ReplaceWithRepresentativeOption(&c));
  EXPECT_TRUE(
      logic.ReplaceWithRepresentativeOption(&longc));

  EXPECT_EQ(expectedc.ToString(), c.ToString());
  EXPECT_EQ(expectedc.ToString(), longc.ToString());
}

TEST(ArgvProcessorTest, OptionIsBound) {
  set<Token> bound_options = {Token(TerminalType::POSIX_OPTION, "-o"),
                              Token(TerminalType::POSIX_OPTION, "-I")};
  ArgvProcessLogic logic({}, {}, bound_options);
  auto c = Token(TerminalType::POSIX_OPTION, "-c");
  auto o = Token(TerminalType::POSIX_OPTION, "-o");
  auto I = Token(TerminalType::POSIX_OPTION, "-I");
  EXPECT_TRUE(logic.OptionIsBound(o));
  EXPECT_TRUE(logic.OptionIsBound(I));
  EXPECT_FALSE(logic.OptionIsBound(c));
}

TEST(ArgvProcessorTest, ProcessOption) {
  map<Token, Token> option_to_rep_option = {
    {Token(TerminalType::POSIX_OPTION, "-c"),
     Token(TerminalType::GNU_OPTION, "--longc")},
    {Token(TerminalType::GNU_OPTION, "--longc"),
     Token(TerminalType::GNU_OPTION, "--longc")},
  };
  ArgvProcessLogic logic({}, option_to_rep_option, {});
  auto longc = Token(TerminalType::GNU_OPTION, "--longc");
  EXPECT_FALSE(logic.ProcessOption(TerminalType::POSIX_OPTION, "-c"));
  EXPECT_EQ(
      longc.ToString(),
      logic.tokens_.front().ToString());
}

TEST(ArgvProcessorTest, GetPreprocessedArguments) {
  const char *argv[] = {
    "utility_name",            // removed.
    "-c",
    "whatever",
    "1 2 3 a b c",             // one unit.

    "-I/usr/whatever",         // -I /usr/whatever
    "--long-option=!@$*$",     // --long-option = !@$*$

    "-I",
    "--long-option=whatever",  // `GENERAL_ELEMENT`.

    "-foFILE",                 // -f -o FILE

    "--",                      // removed.
    "-c",                      // `GENERAL_ELEMENT`.
    "--long",                  // `GENERAL_ELEMENT`.
  };
  ArgvProcessor argv_processor;
  argv_processor.LoadArgv(sizeof(argv) / sizeof(argv[0]), argv);

  map<Token, Token> option_to_rep_option = {
    {Token(TerminalType::POSIX_OPTION, "-c"),
     Token(TerminalType::GNU_OPTION, "--longc")},
    {Token(TerminalType::GNU_OPTION, "--longc"),
     Token(TerminalType::GNU_OPTION, "--longc")},
    {Token(TerminalType::GNU_OPTION, "--long-option"),
     Token(TerminalType::GNU_OPTION, "--long-option")},
    {Token(TerminalType::POSIX_OPTION, "-f"),
     Token(TerminalType::POSIX_OPTION, "-f")},
    {Token(TerminalType::POSIX_OPTION, "-o"),
     Token(TerminalType::POSIX_OPTION, "-o")},
    {Token(TerminalType::POSIX_OPTION, "-I"),
     Token(TerminalType::POSIX_OPTION, "-I")},
  };
  set<Token> bound_options = {
    Token(TerminalType::POSIX_OPTION, "-o"),
    Token(TerminalType::POSIX_OPTION, "-I"),
    Token(TerminalType::GNU_OPTION, "--long-option"),
  };
  auto tokens = argv_processor.GetPreprocessedArguments(
      option_to_rep_option,
      bound_options);
  vector<TerminalType> types;
  vector<string> values;
  for (const auto &token : tokens) {
    types.push_back(token.type());
    values.push_back(token.value());
  }
  vector<TerminalType> expected_types = {
    TerminalType::GNU_OPTION,
    TerminalType::GENERAL_ELEMENT,
    TerminalType::GENERAL_ELEMENT,

    TerminalType::POSIX_OPTION,
    TerminalType::GENERAL_ELEMENT,

    TerminalType::GNU_OPTION,
    TerminalType::GENERAL_ELEMENT,

    TerminalType::POSIX_OPTION,
    TerminalType::GENERAL_ELEMENT,

    TerminalType::POSIX_OPTION,
    TerminalType::POSIX_OPTION,
    TerminalType::GENERAL_ELEMENT,

    TerminalType::GENERAL_ELEMENT,
    TerminalType::GENERAL_ELEMENT,
  };
  EXPECT_EQ(expected_types, types);
  vector<string> expected_values = {
    "--longc",
    "whatever",
    "1 2 3 a b c",

    "-I",
    "/usr/whatever",

    "--long-option",
    "!@$*$",

    "-I",
    "--long-option=whatever",  // `GENERAL_ELEMENT`.

    "-f",
    "-o",
    "FILE",

    "-c",
    "--long",
  };
  EXPECT_EQ(expected_values, values);
}
