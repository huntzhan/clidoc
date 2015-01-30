#include <iostream>
#include <string>
#include <set>
#include <map>

#include "gtest/gtest.h"
#include "ast/ast_build.h"

using std::set;
using std::map;
using std::string;

namespace clidoc {

TEST(ParserProxyTest, Parse) {
  string input =
R"doc(
Usage:
  naval_fate.py -abc -- --whatever
  naval_fate.py --test <multiple>...
  naval_fate.py ship new <name>...
  naval_fate.py ship <name> move <x> <y> [--speed=<kn>]
  naval_fate.py ship shoot <x> <y>
  naval_fate.py mine (set|remove) <x> <y> [--moored | --drifting]
  naval_fate.py (-h | --help)
  naval_fate.py --version

Options:
  -a -b -c                     # Test.
  --test <multiple>
  -h --help                    # Show this screen.
  --version                    # Show version.
  --speed=<kn> [default: "10"] # Speed in knots.
  --moored                     # Moored (anchored) mine.
  --drifting                   # Drifting mine.
)doc";

  CodeGenInfo info;
  info.Prepare(input);

  std::cout << info.doc_node_->ToString(0);

  auto CheckToken = [](set<string> expected, set<Token> actual) {
    set<string> values;
    for (const auto &element : actual) {
      values.insert(element.value());
    }
    EXPECT_EQ(expected, values);
  };
  auto CheckMap = [](map<string, string> expected, map<Token, string> actual) {
    map<string, string> values;
    for (const auto &element : actual) {
      values[element.first.value()] = element.second;
    }
    EXPECT_EQ(expected, values);
  };
  CheckToken({"--speed"}, info.bound_options_);
  CheckToken(
      {
        "--drifting",
        "--help",
        "--moored",
        "--version",
        "-a",
      },
      info.unbound_options_);

  CheckToken(
      {
        "--whatever",
        "<x>",
        "<y>",
      },
      info.arguments_);
  CheckToken(
      {
        "mine",
        "move",
        "new",
        "remove",
        "set",
        "ship",
        "shoot",
      },
      info.commands_);
  CheckToken({"--test"}, info.oom_bound_options_);
  CheckToken({"<name>"}, info.oom_arguments_);
  CheckMap({{"--speed", "10"}}, info.default_values_);
}

}  // namespace clidoc
