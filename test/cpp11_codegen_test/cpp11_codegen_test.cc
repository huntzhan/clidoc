
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "clidoc/ast/ast_build.h"
#include "clidoc/ast/ast_visitor_helper.h"

#include "clidoc/info.h"
#include "clidoc/match_logic.h"

using namespace clidoc;
using std::string;

string example_doc =
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
  --test <multiple>
  -h --help                    # Show this screen.
  --version                    # Show version.
  --speed=<kn> [default: "10"] # Speed in knots.
  --moored                     # Moored (anchored) mine.
  --drifting                   # Drifting mine.
)doc";

TEST(cpp_code_gen, argv_match) {
  // load up `CppCodegenInfo`.
  CodegenInfo info;
  CppCodegenInfo cppinfo;
  info.Prepare(example_doc);
  cppinfo.doc_node_ = info.doc_node_;
  cppinfo.bound_options_ = info.bound_options_;
  cppinfo.unbound_options_ = info.unbound_options_;
  cppinfo.arguments_ = info.arguments_;
  cppinfo.oom_bound_options_ = info.oom_bound_options_;
  cppinfo.oom_arguments_ = info.oom_arguments_;
  cppinfo.commands_ = info.commands_;
  cppinfo.default_values_ = {
    {Token(TerminalType::GNU_OPTION, "--speed"), "10"},
  };
  // std::cout << GenerateSource(info) << std::endl;
  std::cout << cppinfo.doc_node_->ToString(0) << std::endl;
  // test `MatchStrategy`.
  MatchStrategy strategy(
      cppinfo,
      {
        Token(TerminalType::COMMAND, "ship"),
        Token(TerminalType::GENERAL_ELEMENT, "name_of_ship"),
        Token(TerminalType::COMMAND, "move"),
        Token(TerminalType::GENERAL_ELEMENT, "10"),
        Token(TerminalType::GENERAL_ELEMENT, "20"),
        Token(TerminalType::GNU_OPTION, "--speed"),
        Token(TerminalType::GENERAL_ELEMENT, "42"),
      });
  cppinfo.doc_node_->Accept(&strategy);
  EXPECT_TRUE(strategy.AllMatch());
}
