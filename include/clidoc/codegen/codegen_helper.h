#ifndef INCLUDE_CLIDOC_CODEGEN_CODEGEN_HELPER_H_
#define INCLUDE_CLIDOC_CODEGEN_CODEGEN_HELPER_H_

#include <array>
#include <map>
#include <string>
#include <vector>

#include "clidoc/ast/ast_build.h"
#include "clidoc/ast/ast_nodes.h"
#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/ast/ast_visitor_helper.h"

namespace clidoc {

using FormatPair = std::array<std::string, 2>;

class CollectedElementCodeGenerator {
 public:
  virtual ~CollectedElementCodeGenerator() = default;

  virtual std::string GenerateCode(const CodegenInfo &codegen_info) const;

  void SetTokenFormat(
      const std::map<TerminalType, std::string> &token_format);

  void SetBoundOptionsDeclFormat(
      const std::string &decl_format,
      const std::string &element_format);
  void SetUnboundOptionsDeclFormat(
      const std::string &decl_format,
      const std::string &element_format);
  void SetArgumentsDeclFormat(
      const std::string &decl_format,
      const std::string &element_format);
  void SetOOMBoundOptionsDeclFormat(
      const std::string &decl_format,
      const std::string &element_format);
  void SetOOMArgumentsDeclFormat(
      const std::string &decl_format,
      const std::string &element_format);
  void SetCommandsDeclFormat(
      const std::string &decl_format,
      const std::string &element_format);

  void SetDefaultValuesDeclFormat(
      const std::string &decl_format,
      const std::string &element_format);
  void SetOptionToRepresentativeOptionDeclFormat(
      const std::string &decl_format,
      const std::string &element_format);
  void SetDocTextDeclFormat(
      const std::string &decl_format);

 private:
  enum class FocusedElementType {
    BOUND_OPTIONS,
    UNBOUND_OPTIONS,
    ARGUMENTS,
    OOM_BOUND_OPTIONS,
    OOM_ARGUMENTS,
    COMMANDS,
  };

  void AddFocusedElementFormatPair(
      const FocusedElementType &type,
      const std::string &decl_format,
      const std::string &element_format);

  std::map<TerminalType, std::string> token_format_;

  std::map<FocusedElementType, FormatPair> focused_elements_format_pair_;
  FormatPair default_values_format_pair_;
  FormatPair option_to_representative_option_format_pair_;
  std::string doc_text_format_;
};

class ASTCodeGenerator : public VisitorProcessLogic {
 public:
  virtual ~ASTCodeGenerator() = default;

  void SetVariableNameFormat(const std::string &format);
  void SetAddingChildStatFormat(const std::string &format);
  void SetBindingRootNodeStatFormat(const std::string &format);
  void SetNodeDeclFormat(
      const std::map<TerminalType, std::string> &terminal_format,
      const std::map<NonTerminalType, std::string> &non_terminal_format);

  template <TerminalType type>
  void ProcessNode(TerminalTypeSharedPtr<type> node);
  template <NonTerminalType type>
  void ProcessNode(NonTerminalTypeSharedPtr<type> node);

  virtual std::string GenerateCode() const;

 private:
  std::string RenderNodeDecl(
      const std::string &variable_name,
      const TerminalType &type,
      const std::string &value) const;
  std::string RenderNodeDecl(
      const std::string &variable_name,
      const NonTerminalType &type) const;

  std::string RenderAddingChildStat(
      const std::string &parent_variable_name,
      const std::string &child_variable_name) const;

  std::string RenderBindingRootNodeStat(
      const std::string &root_variable_name) const;

  std::string GetRootVariableName() const;
  std::string GenerateVariableNmae();

  std::string variable_name_format_;
  std::string adding_child_stat_format_;
  std::string binding_root_node_stat_format_;

  std::map<TerminalType, std::string> terminal_format_;
  std::map<NonTerminalType, std::string> non_terminal_format_;

  std::string child_variable_name_;
  int varialbe_id = 0;
  std::vector<std::string> expressions_;
};

class CodegenHelper {
 public:
  CodegenHelper(
      const CodegenInfo &codegen_info,
      const CollectedElementCodeGenerator &collected_element_code_generator,
      ASTCodeGenerator *ast_code_generator);

  void SetCodegenPrefixAndSuffix(
      const std::string &codegen_prefix,
      const std::string &codegen_suffix);

  std::string GenerateCode() const;

 private:
  const CodegenInfo &codegen_info_;
  const CollectedElementCodeGenerator &collected_element_code_generator_;
  ASTCodeGenerator *ast_code_generator_;

  std::string codegen_prefix_;
  std::string codegen_suffix_;
};

}  // namespace clidoc

namespace clidoc {

template <TerminalType type>
void ASTCodeGenerator::ProcessNode(TerminalTypeSharedPtr<type> node) {
  std::string variable_name = GenerateVariableNmae();
  std::string stat = RenderNodeDecl(
      variable_name,
      type,
      node->token_.value());
  expressions_.push_back(stat);
  child_variable_name_ = variable_name;
}

template <NonTerminalType type>
void ASTCodeGenerator::ProcessNode(NonTerminalTypeSharedPtr<type> node) {
  // apply processing logic to children, and collect variable name of children.
  std::vector<std::string> child_variable_names;
  for (auto child_node : node->children_) {
    child_node->Accept(visitor_ptr_);
    child_variable_names.push_back(child_variable_name_);
  }

  std::string parent_variable_name = GenerateVariableNmae();
  // generate codes related to parent.
  expressions_.push_back(
      RenderNodeDecl(parent_variable_name, type));

  for (const auto &child_variable_name : child_variable_names) {
    expressions_.push_back(
        RenderAddingChildStat(parent_variable_name, child_variable_name));
  }
  child_variable_name_ = parent_variable_name;
}

}  // namespace clidoc

#endif  // INCLUDE_CLIDOC_CODEGEN_CODEGEN_HELPER_H_
