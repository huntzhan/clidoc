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

TEST(CollectedElementCodeGenerator, GenerateCode) {
  CodegenInfo codegen_info;
  codegen_info.bound_options_ = {
    Token(TerminalType::POSIX_OPTION, "-a"),
  };
  codegen_info.unbound_options_= {
    Token(TerminalType::POSIX_OPTION, "-b"),
  };
  codegen_info.arguments_= {
    Token(TerminalType::ARGUMENT, "ARG1"),
  };
  codegen_info.oom_bound_options_= {
    Token(TerminalType::POSIX_OPTION, "-c"),
  };
  codegen_info.oom_arguments_= {
    Token(TerminalType::ARGUMENT, "ARG2"),
  };
  codegen_info.commands_ = {
    Token(TerminalType::COMMAND, "command"),
  };
  codegen_info.default_values_ = {
    {Token(TerminalType::POSIX_OPTION, "-a"), "42"},
  };
  codegen_info.option_recorder_.option_to_representative_option_ = {
    {Token(TerminalType::POSIX_OPTION, "-b"),
     Token(TerminalType::POSIX_OPTION, "-a")},
  };
  codegen_info.doc_text_ = "whatever";

  CollectedElementCodeGenerator cec_generator;
  const map<TerminalType, string> token_to_string = {
    {TerminalType::POSIX_OPTION, "po<%1%>"},
    {TerminalType::ARGUMENT, "arg<%1%>"},
    {TerminalType::COMMAND, "com<%1%>"},
  };
  cec_generator.SetTokenFormat(token_to_string);

  cec_generator.SetBoundOptionsDeclFormat(
      "bo = {\n%1%};",
      "%1%,");
  cec_generator.SetUnboundOptionsDeclFormat(
      "ubo = {\n%1%};",
      "%1%,");
  cec_generator.SetArgumentsDeclFormat(
      "arg = {\n%1%};",
      "%1%,");
  cec_generator.SetOOMBoundOptionsDeclFormat(
      "oomb = {\n%1%};",
      "%1%,");
  cec_generator.SetOOMArgumentsDeclFormat(
      "ooma = {\n%1%};",
      "%1%,");
  cec_generator.SetCommandsDeclFormat(
      "com = {\n%1%};",
      "%1%,");
  cec_generator.SetDefaultValuesDeclFormat(
      "dv = {\n%1%};",
      "%1%: %2%,");
  cec_generator.SetOptionToRepresentativeOptionDeclFormat(
      "otro = {\n%1%};",
      "%1%: %2%,");
  cec_generator.SetDocTextDeclFormat(
      "doc_text = \"%1%\";");

  string expected = R"doc(bo = {
po<-a>,
};
ubo = {
po<-b>,
};
arg = {
arg<ARG1>,
};
oomb = {
po<-c>,
};
ooma = {
arg<ARG2>,
};
com = {
com<command>,
};
dv = {
po<-a>: 42,
};
otro = {
po<-b>: po<-a>,
};
doc_text = "whatever";
)doc";
  EXPECT_EQ(expected, cec_generator.GenerateCode(codegen_info));
}
