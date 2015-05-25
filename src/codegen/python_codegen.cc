#include "clidoc/codegen/python_codegen.h"

#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include "clidoc/ast/ast_build.h"
#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/codegen/codegen_helper.h"
#include "clidoc/codegen/filesystem.h"

using std::endl;
using std::ifstream;
using std::map;
using std::ostringstream;
using std::set;
using std::string;

const string kIndent = "    ";

#define OSTRM_PROPERTY(data_member)                \
  ostrm << GenerateSetOfToken(                     \
      RemoveUnderscoreSuffix("Info."#data_member), \
      code_gen_info.data_member)                   \

namespace clidoc {

namespace python {

string RemoveUnderscoreSuffix(const string &variable) {
  return string(variable.cbegin(), variable.cend() - 1);
}

string GeneratePythonToken(const Token &token) {
  ostringstream ostrm;
  ostrm << "Token(Token.";
  switch (token.type()) {
    case TerminalType::POSIX_OPTION:
      ostrm << "POSIX_OPTION";
      break;
    case TerminalType::GNU_OPTION:
      ostrm << "GNU_OPTION";
      break;
    case TerminalType::COMMAND:
      ostrm << "COMMAND";
      break;
    case TerminalType::ARGUMENT:
      ostrm << "ARGUMENT";
      break;
    default:
      throw "GenerateSetOfToken";
  }
  ostrm << ", \"" << token.value() << "\")";
  return ostrm.str();
}

string GenerateSetOfToken(
    const string &variable,
    const set<Token> &elements) {
  ostringstream ostrm;
  // assign to `variable`.
  ostrm << variable << " = set([" << endl;
  for (const Token &element : elements) {
    ostrm << kIndent << GeneratePythonToken(element) << "," << endl;
  }
  ostrm << "])" << endl;
  return ostrm.str();
}

string GenerateInitializerList(
    const string &variable,
    const map<Token, string> default_values) {
  ostringstream ostrm;
  ostrm << variable << " = {" << endl;
  for (const auto &map_pair : default_values) {
    ostrm << kIndent
          << GeneratePythonToken(map_pair.first)
          << ": "
          << "\"" << map_pair.second << "\""
          << ","
          << endl;
  }
  ostrm << "}" << endl;
  return ostrm.str();
}

string GenerateInitializerList(
    const string &variable,
    const map<Token, Token> default_values) {
  ostringstream ostrm;
  ostrm << variable << " = {" << endl;
  for (const auto &map_pair : default_values) {
    ostrm << kIndent
          << GeneratePythonToken(map_pair.first)
          << ": "
          << GeneratePythonToken(map_pair.second)
          << ","
          << endl;
  }
  ostrm << "}" << endl;
  return ostrm.str();
}

class PythonCollectedElementCodeGenerator
    : public CollectedElementCodeGenerator {
 public:
  string GenerateCode(const CodeGenInfo &code_gen_info) const override {
    ostringstream ostrm;
    OSTRM_PROPERTY(bound_options_);
    OSTRM_PROPERTY(unbound_options_);
    OSTRM_PROPERTY(arguments_);
    OSTRM_PROPERTY(oom_bound_options_);
    OSTRM_PROPERTY(oom_arguments_);
    OSTRM_PROPERTY(commands_);

    ostrm << GenerateInitializerList(
        "Info.default_values",
        code_gen_info.default_values_);

    ostrm << GenerateInitializerList(
        "Info.option_to_representative_option",
        code_gen_info.option_recorder_.option_to_representative_option_);

    ostrm << "Info.doc_text = '''"
          << code_gen_info.doc_text_
          << "'''"
          << endl;
    return ostrm.str();
  }
};

}  // namespace python

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

  CodegenHelper codegen_helper(
      code_gen_info,
      python::PythonCollectedElementCodeGenerator(),
      &ast_code_generator);
  codegen_helper.SetCodegenPrefixAndSuffix(
      LoadFileFromResource("python/codegen.py"),
      "");
  return codegen_helper.GenerateCode();
}

}  // namespace clidoc
