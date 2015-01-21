#ifndef SRC_CODE_GEN_CPP_CODE_GEN_LOGIC_H_
#define SRC_CODE_GEN_CPP_CODE_GEN_LOGIC_H_

#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <set>

#include "ast/ast_node_interface.h"
#include "ast/ast_visitor_helper.h"

namespace clidoc {

std::string GenerateSetOfToken(
    const std::string &variable,
    const std::set<Token> &elements);

class ASTTextGenerator : public VisitorProcessLogic {
 public:
  template <TerminalType T>
  void ProcessNode(std::shared_ptr<Terminal<T>> node);
  template <NonTerminalType T>
  void ProcessNode(std::shared_ptr<NonTerminal<T>> node);

  std::string GetExpressions() const;

 private:
  std::string child_variable_name_;
  std::vector<std::string> expressions_;
};

}  // namespace clidoc

namespace clidoc {

template <TerminalType T>
void ASTTextGenerator::ProcessNode(std::shared_ptr<Terminal<T>> node) {
  std::string variable_name = "t" + std::to_string(expressions_.size());
  std::string expression = variable_name + " = "
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
      parent_variable_name + " = "
      + node->GetID() + "::Init();";
  expressions_.push_back(expression);
  for (const auto &child_variable_name : child_variable_names) {
    expressions_.push_back(
        parent_variable_name + "->AddChild(" + child_variable_name + ");");
  }
  child_variable_name_ = parent_variable_name;
}


std::string ASTTextGenerator::GetExpressions() const {
  std::ostringstream ostrm;
  for (const auto &expression : expressions_) {
    ostrm << expression << std::endl;
  }
  return ostrm.str();
}

}  // namespace clidoc

#endif  // SRC_CODE_GEN_CPP_CODE_GEN_LOGIC_H_
