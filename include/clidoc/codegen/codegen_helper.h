#ifndef INCLUDE_CLIDOC_CODEGEN_CODEGEN_HELPER_H_
#define INCLUDE_CLIDOC_CODEGEN_CODEGEN_HELPER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "clidoc/ast/ast_build.h"
#include "clidoc/ast/ast_nodes.h"
#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/ast/ast_visitor_helper.h"

namespace clidoc {

class CollectedElementCodeGenerator {
 public:
  virtual std::string GenerateCode(const CodeGenInfo &code_gen_info) const = 0;
};

class ASTCodeGenerator : public VisitorProcessLogic {
 public:
  void SetVariableNameFormat(const std::string &format);
  void SetAddingChildStatFormat(const std::string &format);
  void SetBindingRootNodeStatFormat(const std::string &format);
  void SetNodeDeclFormat(
      const std::map<TerminalType, std::string> &terminal_format,
      const std::map<NonTerminalType, std::string> &non_terminal_format);

  template <TerminalType type>
  void ProcessNode(std::shared_ptr<Terminal<type>> node);
  template <NonTerminalType type>
  void ProcessNode(std::shared_ptr<NonTerminal<type>> node);

  std::string GenerateCode() const;

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
      const CodeGenInfo &code_gen_info,
      const CollectedElementCodeGenerator &collected_element_code_generator,
      ASTCodeGenerator *ast_code_generator);

  void SetCodegenPrefixAndSuffix(
      const std::string &codegen_prefix,
      const std::string &codegen_suffix);

  std::string GenerateCode() const;

 private:
  const CodeGenInfo &code_gen_info_;
  const CollectedElementCodeGenerator &collected_element_code_generator_;
  ASTCodeGenerator *ast_code_generator_;

  std::string codegen_prefix_;
  std::string codegen_suffix_;
};

}  // namespace clidoc

namespace clidoc {

template <TerminalType type>
void ASTCodeGenerator::ProcessNode(std::shared_ptr<Terminal<type>> node) {
  std::string variable_name = GenerateVariableNmae();
  std::string stat = RenderNodeDecl(
      variable_name,
      type,
      node->token_.value());
  expressions_.push_back(stat);
  child_variable_name_ = variable_name;
}

template <NonTerminalType type>
void ASTCodeGenerator::ProcessNode(std::shared_ptr<NonTerminal<type>> node) {
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
