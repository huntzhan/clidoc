#ifndef SRC_AST_PROCESS_LOGIC_H_
#define SRC_AST_PROCESS_LOGIC_H_

#include <memory>
#include <stack>
#include <iterator>

#include "ast/ast_nodes.h"
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
  template <NonTerminalType T>
  void RemoveDuplicatedNodes(std::shared_ptr<NonTerminal<T>> node_ptr);
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
  template <TerminalType T>
  void ChangeType(std::shared_ptr<Terminal<T>> node_ptr);
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

// class FocusedElementCollector : public NodeVistorInterface {
//  public:
//   using NodeVistorInterface::ProcessNode;
// 
//   explicit FocusedElementCollector(OptionBindingRecorder *recorder_ptr);
// 
//  private:
//   OptionBindingRecorder *recorder_ptr_;
// };

}  // namespace clidoc

namespace clidoc {

template <NonTerminalType T>
void StructureOptimizer::RemoveDuplicatedNodes(
    std::shared_ptr<NonTerminal<T>> node_ptr) {
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
    node_ptr->children_.push_back(child_ptr);
    // make connection.
    child_ptr->node_connection.ConnectParent(
        std::prev(node_ptr->children_.end()),
        &node_ptr->children_);
  }
  children_of_child_ = optimized_children;
}

template <typename TargetType>
template <TerminalType T>
void TerminalTypeModifier<TargetType>::ChangeType(
    std::shared_ptr<Terminal<T>> node_ptr) {

  auto new_node = TargetType::Init(node_ptr->token_.value());
  *node_ptr->node_connection.this_iter_ = new_node;

  new_node->node_connection.ConnectParent(
      &node_ptr->node_connection);
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
