#ifndef SRC_AST_PROCESS_LOGIC_H_
#define SRC_AST_PROCESS_LOGIC_H_

#include <iterator>
#include <set>

#include "ast/ast_node_interface.h"
#include "ast/ast_nodes.h"
#include "ast/option_record.h"
#include "ast/parser_proxy.h"

namespace clidoc {

template <typename TargetType>
struct NodeTypeModifier {
  // Could be invoked other visitors.
  template <typename TerminalTypeSharedPtr>
  static void ChangeTerminalType(TerminalTypeSharedPtr node);
  template <typename NonTerminalTypeSharedPtr>
  static void ChangeNonTerminalType(NonTerminalTypeSharedPtr node);
};

class StructureOptimizer : public NodeVistorInterface {
 public:
  using NodeVistorInterface::ProcessNode;

  void ProcessNode(LogicAnd::SharedPtr node) override;
  void ProcessNode(LogicXor::SharedPtr node) override;
  void ProcessNode(LogicOr::SharedPtr node) override;
  void ProcessNode(LogicOptional::SharedPtr node) override;
  void ProcessNode(LogicOneOrMore::SharedPtr node) override;

 private:
  template <typename NodeTypeOfParent, typename NodeTypeOfChild>
  bool CanRemoveChild(NodeTypeOfParent parent_node,
                      NodeTypeOfChild child_node);

  template <typename NonTerminalTypeSharedPtr>
  void ConditionalRemoveChild(NonTerminalTypeSharedPtr node);

  template <typename NonTerminalTypeSharedPtr>
  void ConditionalRemoveParent(NonTerminalTypeSharedPtr node);

  SharedPtrNodeContainer children_of_child_;
};

// class for `DoubleHyphenHandler`.
template <typename TargetType>
class TerminalTypeModifier : public NodeVistorInterface {
 public:
  using NodeVistorInterface::ProcessNode;

  void ProcessNode(KDoubleHyphen::SharedPtr node) override;
  void ProcessNode(KOptions::SharedPtr node) override;
  void ProcessNode(PosixOption::SharedPtr node) override;
  void ProcessNode(GroupedOptions::SharedPtr node) override;
  void ProcessNode(GnuOption::SharedPtr node) override;
  void ProcessNode(Argument::SharedPtr node) override;
  void ProcessNode(Command::SharedPtr node) override;
};

class DoubleHyphenHandler : public NodeVistorInterface {
 public:
  using NodeVistorInterface::ProcessNode;

  // Premise: operands after `--` are place at the same level with `--` in AST.
  void ProcessNode(KDoubleHyphen::SharedPtr double_hyphen_node) override;
};

class AmbiguityHandler : public NodeVistorInterface {
 public:
  using NodeVistorInterface::ProcessNode;

  explicit AmbiguityHandler(OptionBindingRecorder *recorder_ptr);
  void ProcessNode(GroupedOptions::SharedPtr grouped_options_node) override;

 private:
  OptionBindingRecorder *recorder_ptr_;
};

class FocusedElementCollector : public NodeVistorInterface {
 public:
  using NodeVistorInterface::ProcessNode;

  explicit FocusedElementCollector(OptionBindingRecorder *recorder_ptr);
  std::set<Token> GetFocusedElement();

  // See discussion in issue #5.
  void ProcessNode(PosixOption::SharedPtr node) override;
  void ProcessNode(GnuOption::SharedPtr node) override;
  void ProcessNode(Argument::SharedPtr node) override;
  void ProcessNode(Command::SharedPtr node) override;

 private:
  OptionBindingRecorder *recorder_ptr_;
  std::set<Token> operand_candidates_;
};

class BoundArgumentCleaner : public NodeVistorInterface {
 public:
  using NodeVistorInterface::ProcessNode;

  explicit BoundArgumentCleaner(const std::set<Token> &bound_arguments);
  void ProcessNode(Argument::SharedPtr node) override;

 private:
  const std::set<Token> &bound_arguments_;
};

}  // namespace clidoc

namespace clidoc {

template <typename NodeTypeOfParent, typename NodeTypeOfChild>
bool StructureOptimizer::CanRemoveChild(
    NodeTypeOfParent parent_node,
    NodeTypeOfChild child_node) {
  const auto &logic_and_name =
      kNonTermianlClassName.at(NonTerminalType::LOGIC_AND);
  const auto &logic_xor_name =
      kNonTermianlClassName.at(NonTerminalType::LOGIC_XOR);
  const auto &logic_or_name =
      kNonTermianlClassName.at(NonTerminalType::LOGIC_OR);
  // decide expand or not.
  bool child_is_logic_and = child_node->GetID() == logic_and_name;
  bool child_is_logic_xor = child_node->GetID() == logic_xor_name;
  bool child_is_logic_or = child_node->GetID() == logic_or_name;

  bool equivalent = false;
  // 1. equivalent if they have the same id.
  if (parent_node->GetID() == child_node->GetID()) {
    equivalent = true;
  }
  // 2. when child_node is one of (`LogicAnd`, `LogicXor`, `LogicOr`), and
  // child_node->GetSizeOfChildren() equals to 1, remove the child_node.
  if ((child_is_logic_and || child_is_logic_xor || child_is_logic_or)
      && child_node->GetSizeOfChildren() == 1) {
    equivalent = true;
  }
  return equivalent;
}

template <typename NonTerminalTypeSharedPtr>
void StructureOptimizer::ConditionalRemoveChild(
    NonTerminalTypeSharedPtr node) {
  // container for processed elements.
  SharedPtrNodeContainer optimized_children;
  // `child_node` must be a reference, since the value of `child_node` in
  // `node->children_` might change.
  for (auto &child_node : node->children_) {
    child_node->Accept(this);
    if (!child_node) {
      // child node has been removed.
      continue;
    }
    if (CanRemoveChild(node, child_node)) {
      // same non-terminal nodes.
      optimized_children.insert(
          optimized_children.end(),
          children_of_child_.begin(), children_of_child_.end());
    } else {
      optimized_children.push_back(child_node);
    }
  }
  // make a copy of children of current node, which are accessiable to the
  // parent of current node.
  node->children_.clear();
  for (auto child_node : optimized_children) {
    node->AddChild(child_node);
  }
  children_of_child_ = optimized_children;
}

template <typename NonTerminalTypeSharedPtr>
void StructureOptimizer::ConditionalRemoveParent(
    NonTerminalTypeSharedPtr node) {
  if (node->children_.size() == 0) {
    *node->node_connection.this_iter_ = nullptr;
  }
}

template <typename TargetType>
template <typename TerminalTypeSharedPtr>
void NodeTypeModifier<TargetType>::ChangeTerminalType(
    TerminalTypeSharedPtr node) {
  auto new_node = TargetType::Init(node->token_.value());
  node->node_connection.ReplacedWith(new_node);
}

template <typename TargetType>
template <typename NonTerminalTypeSharedPtr>
void NodeTypeModifier<TargetType>::ChangeNonTerminalType(
    NonTerminalTypeSharedPtr node) {
  auto new_node = TargetType::Init();
  for (auto child : node->children_) {
    new_node->AddChild(child);
  }
  node->node_connection.ReplacedWith(new_node);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    KDoubleHyphen::SharedPtr node) {
  NodeTypeModifier<TargetType>::ChangeTerminalType(node);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    KOptions::SharedPtr node) {
  NodeTypeModifier<TargetType>::ChangeTerminalType(node);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    PosixOption::SharedPtr node) {
  NodeTypeModifier<TargetType>::ChangeTerminalType(node);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    GroupedOptions::SharedPtr node) {
  NodeTypeModifier<TargetType>::ChangeTerminalType(node);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    GnuOption::SharedPtr node) {
  NodeTypeModifier<TargetType>::ChangeTerminalType(node);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    Argument::SharedPtr node) {
  NodeTypeModifier<TargetType>::ChangeTerminalType(node);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    Command::SharedPtr node) {
  NodeTypeModifier<TargetType>::ChangeTerminalType(node);
}

}  // namespace clidoc

#endif  //  SRC_AST_PROCESS_LOGIC_H_
