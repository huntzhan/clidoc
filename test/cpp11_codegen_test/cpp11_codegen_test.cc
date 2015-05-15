
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "clidoc/ast/ast_visitor_helper.h"

#include "clidoc/codegen/cpp11_codegen.h"
#include "clidoc/info.h"
#include "clidoc/match_logic.h"

using namespace clidoc;
using namespace clidoc::cpp11;
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

TEST(cpp_code_gen, logic) {
  auto flag1 = CanInvoke<ASTTextGenerator, GnuOption::SharedPtr>::value;
  auto flag2 = CanInvoke<ASTTextGenerator, LogicAnd::SharedPtr>::value;
  EXPECT_TRUE(flag1);
  EXPECT_TRUE(flag2);
}

TEST(cpp_code_gen, ast) {
  auto posix_option = PosixOption::Init("-c");
  auto gnu_option = GnuOption::Init("--long");
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(posix_option);
  logic_and->AddChild(gnu_option);
  ASTTextGenerator code_generator;
  auto visitor = GenerateVisitor<AllNodeVisitor>(&code_generator);
  logic_and->Accept(&visitor);
  string expected =
    "auto t0 = PosixOption::Init(Token(TerminalType::POSIX_OPTION, \"-c\"));\n"
    "auto t1 = GnuOption::Init(Token(TerminalType::GNU_OPTION, \"--long\"));\n"
    "auto nt2 = LogicAnd::Init();\n"
    "nt2->AddChild(t0);\n"
    "nt2->AddChild(t1);\n";
  EXPECT_EQ(expected, code_generator.GetExpressions());
}

TEST(cpp_code_gen, argv_match) {
  // load up `CppCodeGenInfo`.
  CodeGenInfo info;
  CppCodeGenInfo cppinfo;
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
        Token(TerminalType::GENERAL_ELEMENT, "ship"),
        Token(TerminalType::GENERAL_ELEMENT, "name_of_ship"),
        Token(TerminalType::GENERAL_ELEMENT, "move"),
        Token(TerminalType::GENERAL_ELEMENT, "10"),
        Token(TerminalType::GENERAL_ELEMENT, "20"),
        Token(TerminalType::GNU_OPTION, "--speed"),
        Token(TerminalType::GENERAL_ELEMENT, "42"),
      });
  cppinfo.doc_node_->Accept(&strategy);
  EXPECT_TRUE(strategy.AllMatch());
}
