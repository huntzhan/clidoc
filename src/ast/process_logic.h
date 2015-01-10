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

  void ProcessNode(LogicAnd::SharedPtr node) override;
  void ProcessNode(LogicXor::SharedPtr node) override;
  void ProcessNode(LogicOr::SharedPtr node) override;
  void ProcessNode(LogicOptional::SharedPtr node) override;
  void ProcessNode(LogicOneOrMore::SharedPtr node) override;

 private:
  // Since "The nested-name-specifier (everything to the left of the scope
  // resolution operator ::) of a type that was specified using a
  // qualified-id." is one of "Non-deduced contexts",
  // `NonTerminalType<T>::SharedPtr` could not be the function parameter.
  template <typename NonTerminalTypeSharedPtr>
  void RemoveDuplicatedNodes(NonTerminalTypeSharedPtr node);
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

 private:
  template <typename TerminalTypeSharedPtr>
  void ChangeType(TerminalTypeSharedPtr node);
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

template <typename NonTerminalTypeSharedPtr>
void StructureOptimizer::RemoveDuplicatedNodes(
    NonTerminalTypeSharedPtr node) {
  // container for processed elements.
  SharedPtrNodeContainer optimized_children;
  for (auto child : node->children_) {
    child->Accept(this);
    // decide expand or not.
    bool child_is_logic_and =
        child->GetID() == kNonTermianlClassName.at(NonTerminalType::LOGIC_AND);
    bool child_is_logic_xor =
        child->GetID() == kNonTermianlClassName.at(NonTerminalType::LOGIC_XOR);
    bool child_is_logic_or =
        child->GetID() == kNonTermianlClassName.at(NonTerminalType::LOGIC_OR);
    bool equivalent = false;
    // 1. equivalent if they have the same id.
    if (node->GetID() == child->GetID()) {
      equivalent = true;
    }
    // 2. when child is one of (LogicAnd, LogicXor, LogicOr), and
    // child->GetSizeOfChildren() equals to 1, remove the child.
    if ((child_is_logic_and || child_is_logic_xor || child_is_logic_or)
        && child->GetSizeOfChildren() == 1) {
      equivalent = true;
    }
    // canoot be accessed.
    if (equivalent) {
      // same non-terminal nodes.
      optimized_children.insert(
          optimized_children.end(),
          children_of_child_.begin(), children_of_child_.end());
    } else {
      optimized_children.push_back(child);
    }
  }
  // make a copy of children of current node, which are accessiable to the
  // parent of current node.
  node->children_.clear();
  for (auto child : optimized_children) {
    node->AddChild(child);
  }
  children_of_child_ = optimized_children;
}

template <typename TargetType>
template <typename TerminalTypeSharedPtr>
void TerminalTypeModifier<TargetType>::ChangeType(
    TerminalTypeSharedPtr node) {
  auto new_node = TargetType::Init(node->token_.value());
  node->node_connection.ReplacedWith(new_node);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    KDoubleHyphen::SharedPtr node) {
  ChangeType(node);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    KOptions::SharedPtr node) {
  ChangeType(node);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    PosixOption::SharedPtr node) {
  ChangeType(node);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    GroupedOptions::SharedPtr node) {
  ChangeType(node);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    GnuOption::SharedPtr node) {
  ChangeType(node);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    Argument::SharedPtr node) {
  ChangeType(node);
}

template <typename TargetType>
void TerminalTypeModifier<TargetType>::ProcessNode(
    Command::SharedPtr node) {
  ChangeType(node);
}

}  // namespace clidoc

#endif  //  SRC_AST_PROCESS_LOGIC_H_
