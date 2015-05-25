#include "clidoc/codegen/cpp11_codegen.h"

#include <map>
#include <string>

#include "clidoc/ast/ast_build.h"
#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/codegen/codegen_helper.h"

using std::map;
using std::string;

namespace clidoc {

string Cpp11Codegen(const CodeGenInfo &code_gen_info) {
  // codegen of AST.
  ASTCodeGenerator ast_code_generator;
  ast_code_generator.SetVariableNameFormat(
      "node_%1%");

  map<TerminalType, string> terminal_format = {
    {TerminalType::POSIX_OPTION, "auto %1% = PosixOption::Init(\"%2%\");"},
    {TerminalType::GNU_OPTION,   "auto %1% = GnuOption::Init(\"%2%\");"},
    {TerminalType::COMMAND,      "auto %1% = Command::Init(\"%2%\");"},
    {TerminalType::ARGUMENT,     "auto %1% = Argument::Init(\"%2%\");"},
  };
  map<NonTerminalType, string> non_terminal_format = {
    {NonTerminalType::DOC,             "auto %1% = Doc::Init();"},
    {NonTerminalType::LOGIC_AND,       "auto %1% = LogicAnd::Init();"},
    {NonTerminalType::LOGIC_XOR,       "auto %1% = LogicXor::Init();"},
    {NonTerminalType::LOGIC_OR,        "auto %1% = LogicOr::Init();"},
    {NonTerminalType::LOGIC_OPTIONAL,  "auto %1% = LogicOptional::Init();"},
    {NonTerminalType::LOGIC_ONEORMORE, "auto %1% = LogicOneOrMore::Init();"},
  };
  ast_code_generator.SetNodeDeclFormat(terminal_format, non_terminal_format);

  ast_code_generator.SetAddingChildStatFormat(
      "%1%->AddChild(%2%);");
  ast_code_generator.SetBindingRootNodeStatFormat(
      "cpp_code_gen_info.doc_node_ = %1%;");

  string codegen_prefix = R"doc(
#include "clidoc/info.h"
namespace clidoc {
CppCodeGenInfo InitCppCodeGenInfo() {
CppCodeGenInfo cpp_code_gen_info;
)doc";

  string codegen_suffix = R"doc(
return cpp_code_gen_info;
}
CppCodeGenInfo cpp_code_gen_info = InitCppCodeGenInfo();
}  // namespace clidoc
)doc";

  // codegen of collected elements.
  CollectedElementCodeGenerator cec_generator;
  map<TerminalType, string> token_to_string = {
    {TerminalType::POSIX_OPTION, "Token(TerminalType::POSIX_OPTION, \"%1%\")"},
    {TerminalType::GNU_OPTION,   "Token(TerminalType::GNU_OPTION, \"%1%\")"},
    {TerminalType::COMMAND,      "Token(TerminalType::COMMAND, \"%1%\")"},
    {TerminalType::ARGUMENT,     "Token(TerminalType::ARGUMENT, \"%1%\")"},
  };
  cec_generator.SetTokenFormat(token_to_string);

  const string decl_format_suffix = " = {\n%1%};";
  const string focused_element_format = "%1%,";
  cec_generator.SetBoundOptionsDeclFormat(
      "cpp_code_gen_info.bound_options_" + decl_format_suffix,
      focused_element_format);
  cec_generator.SetUnboundOptionsDeclFormat(
      "cpp_code_gen_info.unbound_options_" + decl_format_suffix,
      focused_element_format);
  cec_generator.SetArgumentsDeclFormat(
      "cpp_code_gen_info.arguments_" + decl_format_suffix,
      focused_element_format);
  cec_generator.SetOOMBoundOptionsDeclFormat(
      "cpp_code_gen_info.oom_bound_options_" + decl_format_suffix,
      focused_element_format);
  cec_generator.SetOOMArgumentsDeclFormat(
      "cpp_code_gen_info.oom_arguments_" + decl_format_suffix,
      focused_element_format);
  cec_generator.SetCommandsDeclFormat(
      "cpp_code_gen_info.commands_" + decl_format_suffix,
      focused_element_format);

  cec_generator.SetDefaultValuesDeclFormat(
      "cpp_code_gen_info.default_values_" + decl_format_suffix,
      "{%1%, \"%2%\"},");
  cec_generator.SetOptionToRepresentativeOptionDeclFormat(
      "cpp_code_gen_info.option_to_representative_option_"
      + decl_format_suffix,
      "{%1%, %2%},");
  cec_generator.SetDocTextDeclFormat(
      "cpp_code_gen_info.doc_text_ = R\"doc(%1%)doc\";");

  CodegenHelper codegen_helper(
      code_gen_info,
      cec_generator,
      &ast_code_generator);
  codegen_helper.SetCodegenPrefixAndSuffix(codegen_prefix, codegen_suffix);
  return codegen_helper.GenerateCode();
}

}  // namespace clidoc
