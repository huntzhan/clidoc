#include "clidoc/codegen/python_codegen.h"

#include <map>
#include <string>

#include "clidoc/ast/ast_build.h"
#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/codegen/codegen_helper.h"
#include "clidoc/codegen/filesystem.h"

using std::map;
using std::string;

namespace clidoc {

string PythonCodegen(const CodeGenInfo &code_gen_info) {
  // codegen of AST.
  ASTCodeGenerator ast_code_generator;
  ast_code_generator.SetVariableNameFormat(
      "node_%1%");

  map<TerminalType, string> terminal_format = {
    {TerminalType::POSIX_OPTION, "%1% = PosixOption(\"%2%\")"},
    {TerminalType::GNU_OPTION,   "%1% = GnuOption(\"%2%\")"},
    {TerminalType::COMMAND,      "%1% = Command(\"%2%\")"},
    {TerminalType::ARGUMENT,     "%1% = Argument(\"%2%\")"},
  };
  map<NonTerminalType, string> non_terminal_format = {
    {NonTerminalType::DOC,             "%1% = Doc()"},
    {NonTerminalType::LOGIC_AND,       "%1% = LogicAnd()"},
    {NonTerminalType::LOGIC_XOR,       "%1% = LogicXor()"},
    {NonTerminalType::LOGIC_OR,        "%1% = LogicOr()"},
    {NonTerminalType::LOGIC_OPTIONAL,  "%1% = LogicOptional()"},
    {NonTerminalType::LOGIC_ONEORMORE, "%1% = LogicOneOrMore()"},
  };
  ast_code_generator.SetNodeDeclFormat(terminal_format, non_terminal_format);

  ast_code_generator.SetAddingChildStatFormat(
      "%1%.add_child(%2%)");
  ast_code_generator.SetBindingRootNodeStatFormat(
      "Info.doc_node = %1%");

  // codegen of collected elements.
  CollectedElementCodeGenerator cec_generator;
  map<TerminalType, string> token_to_string = {
    {TerminalType::POSIX_OPTION, "Token(Token.POSIX_OPTION, \"%1%\")"},
    {TerminalType::GNU_OPTION,   "Token(Token.GNU_OPTION, \"%1%\")"},
    {TerminalType::COMMAND,      "Token(Token.COMMAND, \"%1%\")"},
    {TerminalType::ARGUMENT,     "Token(Token.ARGUMENT, \"%1%\")"},
  };
  cec_generator.SetTokenFormat(token_to_string);

  const string focused_element_decl_format_suffix = " = set([\n%1%])";
  const string focused_element_format = "    %1%,";
  cec_generator.SetBoundOptionsDeclFormat(
      "Info.bound_options" + focused_element_decl_format_suffix,
      focused_element_format);
  cec_generator.SetUnboundOptionsDeclFormat(
      "Info.unbound_options" + focused_element_decl_format_suffix,
      focused_element_format);
  cec_generator.SetArgumentsDeclFormat(
      "Info.arguments" + focused_element_decl_format_suffix,
      focused_element_format);
  cec_generator.SetOOMBoundOptionsDeclFormat(
      "Info.oom_bound_options" + focused_element_decl_format_suffix,
      focused_element_format);
  cec_generator.SetOOMArgumentsDeclFormat(
      "Info.oom_arguments" + focused_element_decl_format_suffix,
      focused_element_format);
  cec_generator.SetCommandsDeclFormat(
      "Info.commands" + focused_element_decl_format_suffix,
      focused_element_format);

  const string &mapping_decl_format_suffix = " = {\n%1%}";
  cec_generator.SetDefaultValuesDeclFormat(
      "Info.default_values" + mapping_decl_format_suffix,
      "    %1%: \"%2%\",");
  cec_generator.SetOptionToRepresentativeOptionDeclFormat(
      "Info.option_to_representative_option" + mapping_decl_format_suffix,
      "    %1%: %2%,");
  cec_generator.SetDocTextDeclFormat(
      "Info.doc_text = '''%1%'''");

  CodegenHelper codegen_helper(
      code_gen_info,
      cec_generator,
      &ast_code_generator);
  codegen_helper.SetCodegenPrefixAndSuffix(
      LoadFileFromResource("python/codegen.py"),
      "");
  return codegen_helper.GenerateCode();
}

}  // namespace clidoc
