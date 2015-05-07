
#include <string>
#include <vector>
#include <map>

#include "gtest/gtest.h"
#include "ast/ast_node_interface.h"
#include "cpp11/cmake_cpp11_code_gen_project/src/argv_processor.h"

using std::string;
using std::vector;
using std::map;
using namespace clidoc;

TEST(ArgvProcessorTest, GetPreprocessedArguments) {
  const char *argv[] = {
    "utility_name",     // removed.
    "-c",
    "whatever",
    "1 2 3 a b c",      // one unit.
    "-I/usr/whatever",  // -I /usr/whatever
    "-foFILE",          // -f -o FILE
    "--",               // removed.
    "-c",               // `GENERAL_ELEMENT`.
    "--long",           // `GENERAL_ELEMENT`.
  };
  ArgvProcessor argv_processor;
  argv_processor.LoadArgv(sizeof(argv) / sizeof(argv[0]), argv);

  map<Token, Token> option_to_rep_option = {
    {Token(TerminalType::POSIX_OPTION, "-c"),
     Token(TerminalType::GNU_OPTION, "--longc")},
  };
  auto tokens = argv_processor.GetPreprocessedArguments(
      option_to_rep_option,
      {Token(TerminalType::POSIX_OPTION, "-o"),
       Token(TerminalType::POSIX_OPTION, "-I")});
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
    TerminalType::POSIX_OPTION,
    TerminalType::POSIX_OPTION,
    TerminalType::GENERAL_ELEMENT,
    TerminalType::GENERAL_ELEMENT,
    TerminalType::GENERAL_ELEMENT,
  };
  EXPECT_EQ(expected_types, types);
  vector<string> expected_values = 
      {"--longc", "whatever", "1 2 3 a b c",
       "-I", "/usr/whatever", "-f", "-o", "FILE", "-c", "--long"};
  EXPECT_EQ(expected_values, values);
}
