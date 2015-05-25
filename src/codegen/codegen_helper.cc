#include "clidoc/codegen/codegen_helper.h"

#include <functional>
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

using std::cref;
using std::endl;
using std::map;
using std::ostringstream;
using std::reference_wrapper;
using std::set;
using std::string;
using std::to_string;

namespace clidoc {

void CollectedElementCodeGenerator::AddFocusedElementFormatPair(
      const FocusedElementType &type,
      const string &decl_format,
      const string &element_format) {
  focused_elements_format_pair_[type] = {{decl_format, element_format}};
}

void CollectedElementCodeGenerator::SetTokenFormat(
    const map<TerminalType, string> &token_format) {
  token_format_ = token_format;
}

void CollectedElementCodeGenerator::SetBoundOptionsDeclFormat(
    const string &decl_format,
    const string &element_format) {
  AddFocusedElementFormatPair(
      FocusedElementType::BOUND_OPTIONS, decl_format, element_format);
}

void CollectedElementCodeGenerator::SetUnboundOptionsDeclFormat(
    const string &decl_format,
    const string &element_format) {
  AddFocusedElementFormatPair(
      FocusedElementType::UNBOUND_OPTIONS, decl_format, element_format);
}

void CollectedElementCodeGenerator::SetArgumentsDeclFormat(
    const string &decl_format,
    const string &element_format) {
  AddFocusedElementFormatPair(
      FocusedElementType::ARGUMENTS, decl_format, element_format);
}

void CollectedElementCodeGenerator::SetOOMBoundOptionsDeclFormat(
    const string &decl_format,
    const string &element_format) {
  AddFocusedElementFormatPair(
      FocusedElementType::OOM_BOUND_OPTIONS, decl_format, element_format);
}

void CollectedElementCodeGenerator::SetOOMArgumentsDeclFormat(
    const string &decl_format,
    const string &element_format) {
  AddFocusedElementFormatPair(
      FocusedElementType::OOM_ARGUMENTS, decl_format, element_format);
}

void CollectedElementCodeGenerator::SetCommandsDeclFormat(
    const string &decl_format,
    const string &element_format) {
  AddFocusedElementFormatPair(
      FocusedElementType::COMMANDS, decl_format, element_format);
}

void CollectedElementCodeGenerator::SetDefaultValuesDeclFormat(
    const string &decl_format,
    const string &element_format) {
  default_values_format_pair_ = {{decl_format, element_format}};
}

void CollectedElementCodeGenerator::SetOptionToRepresentativeOptionDeclFormat(
    const string &decl_format,
    const string &element_format) {
  option_to_representative_option_format_pair_ =
      {{decl_format, element_format}};
}

void CollectedElementCodeGenerator::SetDocTextDeclFormat(
    const string &decl_format) {
  doc_text_format_ = decl_format;
}

string CollectedElementCodeGenerator::GenerateCode(
    const CodeGenInfo &code_gen_info) const {
  const map<FocusedElementType,
            reference_wrapper<const set<Token>>> kFocusedElementMapping = {
    {FocusedElementType::BOUND_OPTIONS,
     cref(code_gen_info.bound_options_)},
    {FocusedElementType::UNBOUND_OPTIONS,
     cref(code_gen_info.unbound_options_)},
    {FocusedElementType::ARGUMENTS,
     cref(code_gen_info.arguments_)},
    {FocusedElementType::OOM_BOUND_OPTIONS,
     cref(code_gen_info.oom_bound_options_)},
    {FocusedElementType::OOM_ARGUMENTS,
     cref(code_gen_info.oom_arguments_)},
    {FocusedElementType::COMMANDS,
     cref(code_gen_info.commands_)},
  };

  auto RenderToken = [&](const Token &token) -> string {
    const auto &current_token_format = token_format_.at(token.type());
    return boost::str(
        boost::format(current_token_format) % token.value());
  };

  ostringstream ostrm;
  // codegen for focused elements.
  for (const auto &map_pair : focused_elements_format_pair_) {
    const FocusedElementType &type = map_pair.first;
    const string &decl_format = map_pair.second.at(0);
    const string &element_format = map_pair.second.at(1);
    // render elements.
    ostringstream tmp_ostrm;
    for (const auto &token : kFocusedElementMapping.at(type).get()) {
      tmp_ostrm << boost::format(element_format) % RenderToken(token) << endl;
    }
    // reder declaration.
    ostrm << boost::format(decl_format) % tmp_ostrm.str() << endl;
  }

  ostringstream tmp_ostrm;

  // codgen for default_values_.
  for (const auto &map_pair : code_gen_info.default_values_) {
    tmp_ostrm << boost::format(default_values_format_pair_.at(1))
                 % RenderToken(map_pair.first)
                 % map_pair.second
              << endl;
  }
  ostrm << boost::format(default_values_format_pair_.at(0)) % tmp_ostrm.str()
        << endl;

  // codgen for option_to_representative_option_.
  tmp_ostrm.str("");
  for (const auto &map_pair
       : code_gen_info.option_recorder_.option_to_representative_option_) {
    auto element_format = boost::format(
        option_to_representative_option_format_pair_.at(1));
    tmp_ostrm << element_format
                 % RenderToken(map_pair.first)
                 % RenderToken(map_pair.second)
              << endl;
  }
  ostrm << boost::format(option_to_representative_option_format_pair_.at(0))
           % tmp_ostrm.str()
        << endl;

  // codgen for doc_text_.
  ostrm << boost::format(doc_text_format_) % code_gen_info.doc_text_ << endl;
  return ostrm.str();
}

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
