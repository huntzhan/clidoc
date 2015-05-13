#ifndef INCLUDE_CLIDOC_PYTHON_CODEGEN_LOGIC_H_
#define INCLUDE_CLIDOC_PYTHON_CODEGEN_LOGIC_H_

#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/ast/ast_build.h"

namespace clidoc {

namespace python {

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

std::string RemoveUnderscoreSuffix(const std::string &variable);

std::string GeneratePythonToken(const Token &token);

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

}  // namespace python

}  // namespace clidoc

namespace clidoc {

namespace python {

template <TerminalType T>
void ASTTextGenerator::ProcessNode(std::shared_ptr<Terminal<T>> node) {
  // generate unique variable name for terminal node.
  std::string variable_name = "_t" + std::to_string(expressions_.size());
  // generate assignment expression.
  std::ostringstream ostrm;
  ostrm << variable_name << " = "
        << node->GetID() << "(\"" << node->token_.value() << "\")";
  expressions_.push_back(ostrm.str());
  child_variable_name_ = variable_name;
}

template <NonTerminalType T>
void ASTTextGenerator::ProcessNode(std::shared_ptr<NonTerminal<T>> node) {
  // apply processing logic to children, and collect variable name of children.
  std::vector<std::string> child_variable_names;
  for (auto child_node : node->children_) {
    child_node->Accept(visitor_ptr_);
    child_variable_names.push_back(child_variable_name_);
  }
  // generate unique variable name for non-terminal node.
  std::string parent_variable_name =
      "_nt" + std::to_string(expressions_.size());
  // generate assignment expression.
  std::ostringstream ostrm;
  ostrm << parent_variable_name << " = "
        << node->GetID() << "()";
  expressions_.push_back(ostrm.str());
  // add children.
  for (const auto &child_variable_name : child_variable_names) {
    ostrm.str("");
    ostrm << parent_variable_name
          << ".add_child(" << child_variable_name << ")";
    expressions_.push_back(ostrm.str());
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

}  // namespace python

}  // namespace clidoc

#endif  // INCLUDE_CLIDOC_PYTHON_CODEGEN_LOGIC_H_
