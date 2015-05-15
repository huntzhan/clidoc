#define OSTRM_PROPERTY(data_member)                \
  ostrm << GenerateSetOfToken(                     \
      RemoveUnderscoreSuffix("Info."#data_member), \
      code_gen_info.data_member)                   \

#include "clidoc/codegen/python_codegen.h"

#include <string>
#include <set>
#include <map>
#include <sstream>
#include <fstream>

#include "clidoc/ast/ast_build.h"
#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/codegen/filesystem.h"

using std::endl;
using std::ifstream;
using std::map;
using std::ostringstream;
using std::string;
using std::set;

const string kIndent = "    ";

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

std::string GenerateInitializerList(
    const std::string &variable,
    const std::map<Token, std::string> default_values) {
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

std::string GenerateInitializerList(
    const std::string &variable,
    const std::map<Token, Token> default_values) {
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

string GenerateSource(const CodeGenInfo &code_gen_info) {
  ostringstream ostrm;

  // load codegen.py.
  ostrm << LoadFileFromResource("python/codegen.py") << endl << endl
        << "#####################" << endl
        << "#####  codegen  #####" << endl
        << "#####################" << endl;

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

  ASTTextGenerator ast_text_generator;
  auto visitor = GenerateVisitor<AllNodeVisitor>(&ast_text_generator);
  code_gen_info.doc_node_->Accept(&visitor);
  ostrm << ast_text_generator.GetExpressions()
        << endl;

  ostrm << "Info.doc_node = "
        << ast_text_generator.GetRootVariableName()
        << endl;

  ostrm << "Info.doc_text = '''"
        << code_gen_info.doc_text_
        << "'''"
        << endl;
  return ostrm.str();
}

}  // namespace python

}  // namespace clidoc
