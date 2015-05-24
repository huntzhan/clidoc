#ifndef INCLUDE_CLIDOC_CODEGEN_CPP11_CODEGEN_H_
#define INCLUDE_CLIDOC_CODEGEN_CPP11_CODEGEN_H_

#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "clidoc/ast/ast_build.h"
#include "clidoc/ast/ast_nodes.h"
#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/ast/ast_visitor_helper.h"

namespace clidoc {

namespace cpp11 {

class ASTTextGenerator : public VisitorProcessLogic {
 public:
  template <TerminalType T>
  void ProcessNode(std::shared_ptr<Terminal<T>> node);
  template <NonTerminalType T>
  void ProcessNode(std::shared_ptr<NonTerminal<T>> node);

  std::string GetExpressions() const;
  std::string GetRootVariableName() const;

 private:
  std::string child_variable_name_;
  std::vector<std::string> expressions_;
};

std::string GenerateSetOfToken(
    const std::string &variable,
    const std::set<Token> &elements);

std::string GenerateInitializerList(
    const std::string &variable,
    const std::map<Token, std::string> default_values);
std::string GenerateInitializerList(
    const std::string &variable,
    const std::map<Token, Token> default_values);

std::string GenerateSource(const CodeGenInfo &code_gen_info);

}  // namespace cpp11

}  // namespace clidoc

namespace clidoc {

namespace cpp11 {

template <TerminalType T>
void ASTTextGenerator::ProcessNode(std::shared_ptr<Terminal<T>> node) {
  std::string variable_name = "t" + std::to_string(expressions_.size());
  std::string expression = "auto " + variable_name + " = "
                           + node->GetID() + "::Init("
                           + node->token_.ToString() + ");";
  expressions_.push_back(expression);
  child_variable_name_ = variable_name;
}

template <NonTerminalType T>
void ASTTextGenerator::ProcessNode(std::shared_ptr<NonTerminal<T>> node) {
  std::vector<std::string> child_variable_names;
  for (auto child_node : node->children_) {
    child_node->Accept(visitor_ptr_);
    child_variable_names.push_back(child_variable_name_);
  }
  std::string parent_variable_name =
      "nt" + std::to_string(expressions_.size());
  std::string expression =
      "auto " + parent_variable_name + " = "
      + node->GetID() + "::Init();";
  expressions_.push_back(expression);
  for (const auto &child_variable_name : child_variable_names) {
    expressions_.push_back(
        parent_variable_name + "->AddChild(" + child_variable_name + ");");
  }
  child_variable_name_ = parent_variable_name;
}

inline std::string ASTTextGenerator::GetExpressions() const {
  std::ostringstream ostrm;
  for (const auto &expression : expressions_) {
    ostrm << expression << std::endl;
  }
  return ostrm.str();
}

inline std::string ASTTextGenerator::GetRootVariableName() const {
  return child_variable_name_;
}

}  // namespace cpp11

}  // namespace clidoc

#endif  // INCLUDE_CLIDOC_CODEGEN_CPP11_CODEGEN_H_
