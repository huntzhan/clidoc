#ifndef SRC_AST_PROCESS_LOGIC_H_
#define SRC_AST_PROCESS_LOGIC_H_

#include <iterator>
#include <set>

#include "ast/ast_node_interface.h"
#include "ast/ast_nodes.h"
#include "ast/option_record.h"
#include "ast/parser_proxy.h"

namespace clidoc {

class StructureOptimizer : public NodeVistorInterface {
 public:
  using NodeVistorInterface::ProcessNode;

  void ProcessNode(Doc::SharedPtr node_ptr) override;
  void ProcessNode(LogicAnd::SharedPtr node_ptr) override;
  void ProcessNode(LogicXor::SharedPtr node_ptr) override;

 private:
  // Since "The nested-name-specifier (everything to the left of the scope
  // resolution operator ::) of a type that was specified using a
  // qualified-id." is one of "Non-deduced contexts",
  // `NonTerminalType<T>::SharedPtr` could not be the function parameter.
  template <typename NonTerminalTypeSharedPtr>
  void RemoveDuplicatedNodes(NonTerminalTypeSharedPtr node_ptr);
  SharedPtrNodeContainer children_of_child_;
};

// class for `DoubleHyphenHandler`.
template <typename TargetType>
class TerminalTypeModifier : public NodeVistorInterface {
 public:
  using NodeVistorInterface::ProcessNode;

  void ProcessNode(KDoubleHyphen::SharedPtr node_ptr) override;
  void ProcessNode(KOptions::SharedPtr node_ptr) override;
  void ProcessNode(PosixOption::SharedPtr node_ptr) override;
  void ProcessNode(GroupedOptions::SharedPtr node_ptr) override;
  void ProcessNode(GnuOption::SharedPtr node_ptr) override;
  void ProcessNode(Argument::SharedPtr node_ptr) override;
  void ProcessNode(Command::SharedPtr node_ptr) override;

 private:
  template <typename TerminalTypeSharedPtr>
  void ChangeType(TerminalTypeSharedPtr node_ptr);
};


class DoubleHyphenHandler : public NodeVistorInterface {
 public:
  using NodeVistorInterface::ProcessNode;

  // Premise: operands after `--` are place at the same level with `--` in AST.
  void ProcessNode(KDoubleHyphen::SharedPtr double_hyphen_ptr) override;
};

class AmbiguityHandler : public NodeVistorInterface {
 public:
  using NodeVistorInterface::ProcessNode;

  explicit AmbiguityHandler(OptionBindingRecorder *recorder_ptr);
  void ProcessNode(GroupedOptions::SharedPtr grouped_option_ptr) override;

 private:
  OptionBindingRecorder *recorder_ptr_;
};

class FocusedElementCollector : public NodeVistorInterface {
 public:
  using NodeVistorInterface::ProcessNode;

  explicit FocusedElementCollector(OptionBindingRecorder *recorder_ptr);
  std::set<Token> GetFocusedElement();

  // See discussion in issue #5.
  void ProcessNode(PosixOption::SharedPtr node_ptr) override;
  void ProcessNode(GnuOption::SharedPtr node_ptr) override;
  void ProcessNode(Argument::SharedPtr node_ptr) override;

 private:
  OptionBindingRecorder *recorder_ptr_;
  std::set<Token> operand_candidates_;
};

class BoundArgumentCleaner : public NodeVistorInterface {
 public:
  using NodeVistorInterface::ProcessNode;

  explicit BoundArgumentCleaner(const std::set<Token> &bound_arguments);
  void ProcessNode(Argument::SharedPtr node_ptr) override;

 private:
  const std::set<Token> &bound_arguments_;
};

}  // namespace clidoc

namespace clidoc {

template <typename NonTerminalTypeSharedPtr>
void StructureOptimizer::RemoveDuplicatedNodes(
    NonTerminalTypeSharedPtr node_ptr) {
  // container for processed elements.
  SharedPtrNodeContainer optimized_children;
  for (auto child_ptr : node_ptr->children_) {
    child_ptr->Accept(this);
    if (child_ptr->GetID() == node_ptr->GetID()) {
      // same non-terminal nodes.
      optimized_children.insert(
          optimized_children.end(),
          children_of_child_.begin(), children_of_child_.end());
    } else {
      optimized_children.push_back(child_ptr);
    }
  }
  // make a copy of children of current node, which are accessiable to the
  // parent of current node.
  node_ptr->children_.clear();
  for (auto child_ptr : optimized_children) {
    node_ptr->AddChild(child_ptr);
  }
  children_of_child_ = optimized_children;
}

template <typename TargetType>
template <typename TerminalTypeSharedPtr>
void TerminalTypeModifier<TargetType>::ChangeType(
    TerminalTypeSharedPtr node_ptr) {
  auto new_node = TargetType::Init(node_ptr->token_.value());
  node_ptr->node_connection.ReplacedWith(new_node);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    KDoubleHyphen::SharedPtr node_ptr) {
  ChangeType(node_ptr);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    KOptions::SharedPtr node_ptr) {
  ChangeType(node_ptr);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    PosixOption::SharedPtr node_ptr) {
  ChangeType(node_ptr);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    GroupedOptions::SharedPtr node_ptr) {
  ChangeType(node_ptr);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    GnuOption::SharedPtr node_ptr) {
  ChangeType(node_ptr);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    Argument::SharedPtr node_ptr) {
  ChangeType(node_ptr);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    Command::SharedPtr node_ptr) {
  ChangeType(node_ptr);
}

}  // namespace clidoc

#endif  //  SRC_AST_PROCESS_LOGIC_H_
