#ifndef INCLUDE_CLIDOC_CODEGEN_CODEGEN_HELPER_H_
#define INCLUDE_CLIDOC_CODEGEN_CODEGEN_HELPER_H_

#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "clidoc/ast/ast_build.h"
#include "clidoc/ast/ast_nodes.h"
#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/ast/ast_visitor_helper.h"

namespace clidoc {

class CollectedElementCodeGenerator {
  std::string GenerateCode(const CodeGenInfo &code_gen_info) const = 0;
};

class ASTCodeGenerator : public VisitorProcessLogic {
 public:
  void SetVariableNameComponents(
      const std::string &component_1,
      const std::string &component_2);

  void SetNodeDeclComponents(
      const std::string &decl_prefix,
      const std::map<TerminalType, std::string> &terminal_decl_suffix,
      const std::map<NonTerminalType, std::string> &non_terminal_decl_suffix);

  void SetAddingChildStatComponents(
      const std::string &component_1,
      const std::string &component_2,
      const std::string &component_3);

  void SetBindingRootNodeStatComponents(
      const std::string &component_1,
      const std::string &component_2);

  template <TerminalType T>
  void ProcessNode(std::shared_ptr<Terminal<T>> node);
  template <NonTerminalType T>
  void ProcessNode(std::shared_ptr<NonTerminal<T>> node);

  std::string GenerateCode() const;

 private:
  std::string GetRootVariableName() const;
  std::string GenerateVariableNmae();

  std::string variable_name_prefix_;
  std::string variable_name_suffix_;

  std::string decl_prefix_;
  std::map<TerminalType, std::string> terminal_decl_suffix_;
  std::map<NonTerminalType, std::string> non_terminal_decl_suffix_;

  std::string adding_child_stat_component_1_;
  std::string adding_child_stat_component_2_;
  std::string adding_child_stat_component_3_;

  std::string binding_root_node_stat_component_1_;
  std::string binding_root_node_stat_component_2_;

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

  void SetCodegenComponents(
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

inline void ASTCodeGenerator::SetVariableNameComponents(
    const std::string &component_1,
    const std::string &component_2) {
  variable_name_prefix_ = component_1;
  variable_name_suffix_ = component_2;
}

inline void ASTCodeGenerator::SetNodeDeclComponents(
    const std::string &decl_prefix,
    const std::map<TerminalType, std::string> &terminal_decl_suffix,
    const std::map<NonTerminalType, std::string> &non_terminal_decl_suffix) {
  decl_prefix_ = decl_prefix;
  terminal_decl_suffix_ = terminal_decl_suffix;
  non_terminal_decl_suffix_ = non_terminal_decl_suffix;
}

inline void ASTCodeGenerator::SetAddingChildStatComponents(
    const std::string &component_1,
    const std::string &component_2,
    const std::string &component_3) {
  adding_child_stat_component_1_ = component_1;
  adding_child_stat_component_2_ = component_2;
  adding_child_stat_component_3_ = component_3;
}

inline void ASTCodeGenerator::SetAddingChildStatComponents(
    const std::string &component_1,
    const std::string &component_2) {
  binding_root_node_stat_component_1_ = component_1;
  binding_root_node_stat_component_2_ = component_2;
}

inline std::string ASTCodeGenerator::GenerateVariableNmae() {
  std::string variable_name = variable_name_prefix_
                              + std::to_string(varialbe_id++)
                              + variable_name_suffix_;
  return variable_name;
}

template <TerminalType T>
void ASTCodeGenerator::ProcessNode(std::shared_ptr<Terminal<T>> node) {
  std::string variable_name = GenerateVariableNmae();
  expressions_.push_back(
      variable_name
      + terminal_decl_suffix_.at(node->token_.type()));
  child_variable_name_ = variable_name;
}

template <NonTerminalType T>
void ASTCodeGenerator::ProcessNode(std::shared_ptr<NonTerminal<T>> node) {
  // apply processing logic to children, and collect variable name of children.
  std::vector<std::string> child_variable_names;
  for (auto child_node : node->children_) {
    child_node->Accept(visitor_ptr_);
    child_variable_names.push_back(child_variable_name_);
  }

  std::string parent_variable_name = GenerateVariableNmae();
  // generate codes related to parent.
  std::ostringstream ostrm;
  ostrm << parent_variable_name
        << non_terminal_decl_suffix_.at(node->token_.type());
  expressions_.push_back(ostrm.str());

  for (const auto &child_variable_name : child_variable_names) {
    ostrm.str("");
    ostrm << adding_child_stat_component_1_
          << parent_variable_name
          << adding_child_stat_component_2_
          << child_variable_name
          << adding_child_stat_component_3_;
    expressions_.push_back(ostrm.str());
  }
  child_variable_name_ = parent_variable_name;
}

inline std::string ASTCodeGenerator::GenerateCode() const {
  std::ostringstream ostrm;
  // building AST.
  for (const auto &expression : expressions_) {
    ostrm << expression << std::endl;
  }
  // binding root of AST.
  ostrm << binding_root_node_stat_component_1_
        << GetRootVariableName()
        << binding_root_node_stat_component_2_
        << std::endl;
  return ostrm.str();
}

inline std::string ASTCodeGenerator::GetRootVariableName() const {
  return child_variable_name_;
}

CodegenHelper(
    const CodeGenInfo &code_gen_info,
    const CollectedElementCodeGenerator &collected_element_code_generator,
    ASTCodeGenerator *ast_code_generator)
    :
    code_gen_info_(code_gen_info),
    collected_element_code_generator_(collected_element_code_generator),
    ast_code_generator_(ast_code_generator) { /* empty */ }

inline void CodegenHelper::SetCodegenComponents(
    const std::string &codegen_prefix,
    const std::string &codegen_suffix) {
  codegen_prefix_ = codegen_prefix;
  codegen_suffix_ = codegen_suffix;
}

inline std::string CodegenHelper::GenerateCode() const {
  // prepare codgen of AST.
  ASTCodeGenerator ast_code_generator;
  code_gen_info_.doc_node_.Accept(
      &GenerateVisitor<AllNodeVisitor>(&ast_code_generator));

  std::ostringstream ostrm;
  ostrm << codegen_prefix_
        << collected_element_code_generator_.GenerateCode(code_gen_info_)
        << ast_code_generator.GenerateCode()
        << codegen_suffix_;
  return ostrm.str();
}

}  // namespace clidoc

#endif  // INCLUDE_CLIDOC_CODEGEN_CODEGEN_HELPER_H_
