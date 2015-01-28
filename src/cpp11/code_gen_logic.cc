#include <string>
#include <set>
#include <map>
#include <sstream>

#include "ast/ast_build.h"
#include "ast/ast_node_interface.h"
#include "ast/ast_visitor_helper.h"
#include "cpp11/code_gen_logic.h"

using std::string;
using std::set;
using std::map;
using std::ostringstream;
using std::endl;

#define OSTRM_PROPERTY(data_member)     \
  ostrm << GenerateSetOfToken(          \
      "cpp_code_gen_info."#data_member, \
      code_gen_info.data_member)        \

namespace clidoc {

string GenerateSetOfToken(
    const std::string &variable,
    const std::set<Token> &elements) {
  ostringstream ostrm;
  // assign to `variable`.
  ostrm << variable << " = {" << endl;
  for (const Token &element : elements) {
    ostrm << "  " << element.ToString() << "," << endl;
  }
  ostrm << "};" << endl;
  return ostrm.str();
}

std::string GenerateInitializerList(
    const std::string &variable,
    const std::map<Token, std::string> default_values) {
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

std::string GenerateInitializerList(
    const std::string &variable,
    const std::map<Token, Token> default_values) {
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

string GenerateSource(const CodeGenInfo &code_gen_info) {
  ostringstream ostrm;
  ostrm << "#include \"cpp11/info.h\"" << endl
        << "namespace clidoc {" << endl
        << "CppCodeGenInfo InitCppCodeGenInfo() {" << endl
        << "CppCodeGenInfo cpp_code_gen_info;" << endl;

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
      code_gen_info.recorder_.option_to_representative_option_);

  ASTTextGenerator ast_text_generator;
  auto visitor = GenerateVisitor<AllNodeVisitor>(&ast_text_generator);
  code_gen_info.doc_node_->Accept(&visitor);
  ostrm << ast_text_generator.GetExpressions();
  ostrm << "cpp_code_gen_info.doc_node_ = "
        << ast_text_generator.GetRootVariableName()
        << ";" << endl;

  ostrm << "cpp_code_gen_info.doc_text_ = R\"doc("
        << code_gen_info.doc_text_
        << ")doc\";"
        << endl;

  ostrm << "return cpp_code_gen_info;" << endl
        << "}" << endl
        << "CppCodeGenInfo cpp_code_gen_info = InitCppCodeGenInfo();" << endl
        << "}  // namespace clidoc";
  return ostrm.str();
}

}  // namespace clidoc
