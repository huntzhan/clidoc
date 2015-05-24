#include "clidoc/codegen/codegen_helper.h"

#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "boost/format.hpp"
#include "clidoc/ast/ast_build.h"
#include "clidoc/ast/ast_nodes.h"
#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/ast/ast_visitor_helper.h"

using std::endl;
using std::map;
using std::ostringstream;
using std::string;
using std::to_string;

namespace clidoc {

void ASTCodeGenerator::SetVariableNameFormat(
    const string &format) {
  variable_name_format_ = format;
}

void ASTCodeGenerator::SetNodeDeclFormat(
    const map<TerminalType, string> &terminal_format,
    const map<NonTerminalType, string> &non_terminal_format) {
  terminal_format_ = terminal_format;
  non_terminal_format_ = non_terminal_format;
}

void ASTCodeGenerator::SetAddingChildStatFormat(
    const string &format) {
  adding_child_stat_format_ = format;
}

void ASTCodeGenerator::SetBindingRootNodeStatFormat(
    const string &format) {
  binding_root_node_stat_format_ = format;
}

string ASTCodeGenerator::RenderNodeDecl(
    const string &variable_name,
    const TerminalType &type,
    const string &value) const {
  return boost::str(
      boost::format(terminal_format_.at(type)) % variable_name % value);
}

string ASTCodeGenerator::RenderNodeDecl(
    const string &variable_name,
    const NonTerminalType &type) const {
  return boost::str(
      boost::format(non_terminal_format_.at(type)) % variable_name);
}

string ASTCodeGenerator::RenderAddingChildStat(
    const string &parent_variable_name,
    const string &child_variable_name) const {
  return boost::str(
      boost::format(adding_child_stat_format_)
      % parent_variable_name % child_variable_name);
}

string ASTCodeGenerator::RenderBindingRootNodeStat(
    const string &root_variable_name) const {
  return boost::str(
      boost::format(binding_root_node_stat_format_) % root_variable_name);
}

string ASTCodeGenerator::GenerateVariableNmae() {
  return boost::str(
      boost::format(variable_name_format_) % to_string(varialbe_id++));
}

string ASTCodeGenerator::GenerateCode() const {
  ostringstream ostrm;
  // building AST.
  for (const auto &expression : expressions_) {
    ostrm << expression << endl;
  }
  // binding root of AST.
  string stat = boost::str(
      boost::format(binding_root_node_stat_format_) % GetRootVariableName());
  ostrm << stat << endl;
  return ostrm.str();
}

string ASTCodeGenerator::GetRootVariableName() const {
  return child_variable_name_;
}

CodegenHelper::CodegenHelper(
    const CodeGenInfo &code_gen_info,
    const CollectedElementCodeGenerator &collected_element_code_generator,
    ASTCodeGenerator *ast_code_generator)
    :
    code_gen_info_(code_gen_info),
    collected_element_code_generator_(collected_element_code_generator),
    ast_code_generator_(ast_code_generator) { /* empty */ }

void CodegenHelper::SetCodegenPrefixAndSuffix(
    const string &codegen_prefix,
    const string &codegen_suffix) {
  codegen_prefix_ = codegen_prefix;
  codegen_suffix_ = codegen_suffix;
}

string CodegenHelper::GenerateCode() const {
  // prepare codgen of AST.
  auto visitor = GenerateVisitor<AllNodeVisitor>(ast_code_generator_);
  code_gen_info_.doc_node_->Accept(&visitor);

  ostringstream ostrm;
  ostrm << codegen_prefix_
        << collected_element_code_generator_.GenerateCode(code_gen_info_)
        << ast_code_generator_->GenerateCode()
        << codegen_suffix_;
  return ostrm.str();
}

}  // namespace clidoc
