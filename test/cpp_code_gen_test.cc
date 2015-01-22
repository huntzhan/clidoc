
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "code_gen/cpp/code_gen_logic.h"
#include "ast/ast_visitor_helper.h"

using namespace clidoc;
using std::string;

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
    "t0 = PosixOption::Init(Token(TerminalType::POSIX_OPTION, \"-c\"));\n"
    "t1 = GnuOption::Init(Token(TerminalType::GNU_OPTION, \"--long\"));\n"
    "nt2 = LogicAnd::Init();\n"
    "nt2->AddChild(t0);\n"
    "nt2->AddChild(t1);\n";
  EXPECT_EQ(expected, code_generator.GetExpressions());
}

TEST(cpp_code_gen, source) {
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

  std::cout << GenerateSource(info) << std::endl;
}
