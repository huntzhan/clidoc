#include <set>
#include <string>

#include "gtest/gtest.h"
#include "clidoc/ast/ast_nodes.h"
#include "clidoc/ast/ast_visitor_helper.h"

#include "clidoc/codegen/python_codegen.h"

using std::set;
using std::string;
using namespace clidoc;
using namespace clidoc::python;

TEST(python_code_gen, ast) {
  auto posix_option = PosixOption::Init("-c");
  auto gnu_option = GnuOption::Init("--long");
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(posix_option);
  logic_and->AddChild(gnu_option);

  ASTTextGenerator code_generator;
  auto visitor = GenerateVisitor<AllNodeVisitor>(&code_generator);
  logic_and->Accept(&visitor);

  string expected =
    "_t0 = PosixOption(\"-c\")\n"
    "_t1 = GnuOption(\"--long\")\n"
    "_nt2 = LogicAnd()\n"
    "_nt2.add_child(_t0)\n"
    "_nt2.add_child(_t1)\n";
  EXPECT_EQ(expected, code_generator.GetExpressions());
}

TEST(python_code_gen, RemoveUnderscoreSuffix) {
  EXPECT_EQ("command", RemoveUnderscoreSuffix("command_"));
}

TEST(python_code_gen, GeneratePythonToken) {
  EXPECT_EQ(
      "Token(Token.POSIX_OPTION, \"-a\")",
      GeneratePythonToken(Token(TerminalType::POSIX_OPTION, "-a")));
  EXPECT_EQ(
      "Token(Token.GNU_OPTION, \"--long-option\")",
      GeneratePythonToken(Token(TerminalType::GNU_OPTION, "--long-option")));
  EXPECT_EQ(
      "Token(Token.COMMAND, \"add\")",
      GeneratePythonToken(Token(TerminalType::COMMAND, "add")));
  EXPECT_EQ(
      "Token(Token.ARGUMENT, \"<arg>\")",
      GeneratePythonToken(Token(TerminalType::ARGUMENT, "<arg>")));
}
