#include <map>
#include <string>

#include "gtest/gtest.h"
#include "clidoc/ast/ast_nodes.h"
#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/ast/ast_visitor_helper.h"
#include "clidoc/codegen/codegen_helper.h"

using namespace std;
using namespace clidoc;

TEST(ASTCodeGenerator, GenerateCode) {
  // prepare AST for test.
  auto posix_option = PosixOption::Init("-c");
  auto gnu_option = GnuOption::Init("--long");
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(posix_option);
  logic_and->AddChild(gnu_option);

  ASTCodeGenerator ast_code_generator;
  ast_code_generator.SetVariableNameFormat(
      "node%1%");
  ast_code_generator.SetAddingChildStatFormat(
      "%1%->AddChild(%2%);");
  ast_code_generator.SetBindingRootNodeStatFormat(
      "whatever.doc_node_ = %1%;");
  map<TerminalType, string> terminal_format = {
    {TerminalType::POSIX_OPTION, "c1 %1% POSIX_OPTION %2% c2"},
    {TerminalType::GNU_OPTION, "c1 %1% GNU_OPTION %2% c2"},
  };
  map<NonTerminalType, string> non_terminal_format = {
    {NonTerminalType::LOGIC_AND, "c1 %1% LOGIC_AND c2"},
  };
  ast_code_generator.SetNodeDeclFormat(terminal_format, non_terminal_format);

  auto visitor = GenerateVisitor<AllNodeVisitor>(&ast_code_generator);
  logic_and->Accept(&visitor);
  string expected =
    "c1 node0 POSIX_OPTION -c c2\n"
    "c1 node1 GNU_OPTION --long c2\n"
    "c1 node2 LOGIC_AND c2\n"
    "node2->AddChild(node0);\n"
    "node2->AddChild(node1);\n"
    "whatever.doc_node_ = node2;\n";
  EXPECT_EQ(expected, ast_code_generator.GenerateCode());
}
