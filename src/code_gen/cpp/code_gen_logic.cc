#include <string>
#include <set>
#include <sstream>

#include "ast/ast_build.h"
#include "ast/ast_node_interface.h"
#include "ast/ast_visitor_helper.h"
#include "code_gen/cpp/code_gen_logic.h"

using std::string;
using std::set;
using std::ostringstream;
using std::endl;

#define OSTRM_PROPERTY(data_member)      \
  ostrm << GenerateSetOfToken(           \
      "cpp_code_gen_info."#data_member,  \
      code_gen_info.data_member)         \

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

string GenerateSource(const CodeGenInfo &code_gen_info) {
  ostringstream ostrm;
  ostrm << "#include \"code_gen/cpp/info.h\"" << endl
        << "namespace clidoc {" << endl
        << "CppCodeGenInfo cpp_code_gen_info;" << endl;

  OSTRM_PROPERTY(focused_bound_options_);
  OSTRM_PROPERTY(focused_unbound_options_);
  OSTRM_PROPERTY(focused_arguments_);
  OSTRM_PROPERTY(focused_oom_bound_options_);
  OSTRM_PROPERTY(focused_oom_arguments_);
  OSTRM_PROPERTY(focused_commands_);

  ASTTextGenerator ast_text_generator;
  auto visitor = GenerateVisitor<AllNodeVisitor>(&ast_text_generator);
  code_gen_info.doc_node_->Accept(&visitor);
  ostrm << ast_text_generator.GetExpressions();
  ostrm << "cpp_code_gen_info.doc_node_ = "
        << ast_text_generator.GetRootVariableName()
        << ";" << endl;

  ostrm << "cpp_code_gen_info.doc_text_ = R\"doc("
        << code_gen_info.doc_text_
        << ")doc\""
        << endl;

  ostrm << "}  // namespace clidoc";
  return ostrm.str();
}

}  // namespace clidoc
