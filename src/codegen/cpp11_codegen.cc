#include "clidoc/codegen/cpp11_codegen.h"

#include <map>
#include <set>
#include <sstream>
#include <string>

#include "clidoc/ast/ast_build.h"
#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/codegen/codegen_helper.h"

using std::endl;
using std::map;
using std::ostringstream;
using std::set;
using std::string;

#define OSTRM_PROPERTY(data_member)     \
  ostrm << GenerateSetOfToken(          \
      "cpp_code_gen_info."#data_member, \
      code_gen_info.data_member)        \

namespace clidoc {

namespace cpp11 {

string GenerateSetOfToken(
    const string &variable,
    const set<Token> &elements) {
  ostringstream ostrm;
  // assign to `variable`.
  ostrm << variable << " = {" << endl;
  for (const Token &element : elements) {
    ostrm << "  " << element.ToString() << "," << endl;
  }
  ostrm << "};" << endl;
  return ostrm.str();
}

string GenerateInitializerList(
    const string &variable,
    const map<Token, string> default_values) {
  ostringstream ostrm;
  ostrm << variable << " = {" << endl;
  for (const auto &map_pair : default_values) {
    ostrm << "  {"
          << map_pair.first.ToString()
          << ", "
          << "\"" << map_pair.second << "\""
          << "}," << endl;
  }
  ostrm << "};" << endl;
  return ostrm.str();
}

string GenerateInitializerList(
    const string &variable,
    const map<Token, Token> default_values) {
  ostringstream ostrm;
  ostrm << variable << " = {" << endl;
  for (const auto &map_pair : default_values) {
    ostrm << "  {"
          << map_pair.first.ToString()
          << ", "
          << map_pair.second.ToString()
          << "}," << endl;
  }
  ostrm << "};" << endl;
  return ostrm.str();
}

class Cpp11CollectedElementCodeGenerator
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
        "cpp_code_gen_info.default_values_",
        code_gen_info.default_values_);

    ostrm << GenerateInitializerList(
        "cpp_code_gen_info.option_to_representative_option_",
        code_gen_info.option_recorder_.option_to_representative_option_);

    ostrm << "cpp_code_gen_info.doc_text_ = R\"doc("
          << code_gen_info.doc_text_
          << ")doc\";"
          << endl;
    return ostrm.str();
  }
};

} // namespace cpp11

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

  CodegenHelper codegen_helper(
      code_gen_info,
      cpp11::Cpp11CollectedElementCodeGenerator(),
      &ast_code_generator);
  codegen_helper.SetCodegenPrefixAndSuffix(codegen_prefix, codegen_suffix);
  return codegen_helper.GenerateCode();
}

}  // namespace clidoc
