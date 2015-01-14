#ifndef SRC_AST_PROCESS_LOGIC_H_
#define SRC_AST_PROCESS_LOGIC_H_

#include <iterator>
#include <set>

#include "ast/ast_node_interface.h"
#include "ast/ast_nodes.h"
#include "ast/option_record.h"
#include "ast/parser_proxy.h"
#include "ast/ast_visitor_helper.h"

namespace clidoc {

class StructureOptimizerLogic : public VisitorProcessLogic {
 public:
  template <typename NonTerminalTypeSharedPtr>
  void ProcessNode(NonTerminalTypeSharedPtr node);

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

template <typename TargetType>
class TerminalTypeModifierLogic : public VisitorProcessLogic {
 public:
  template <typename NonTerminalTypeSharedPtr>
  void ProcessNode(NonTerminalTypeSharedPtr node);
};

class DoubleHyphenHandler : public NodeVisitorInterface {
 public:
  using NodeVisitorInterface::ProcessNode;

  // Premise: operands after `--` are place at the same level with `--` in AST.
  void ProcessNode(KDoubleHyphen::SharedPtr double_hyphen_node) override;
};

class AmbiguityHandler : public NodeVisitorInterface {
 public:
  using NodeVisitorInterface::ProcessNode;

  explicit AmbiguityHandler(OptionBindingRecorder *recorder_ptr);
  void ProcessNode(GroupedOptions::SharedPtr grouped_options_node) override;

 private:
  OptionBindingRecorder *recorder_ptr_;
};

class NodeRecorderLogic : public VisitorProcessLogic {
 public:
  template <typename NonTerminalTypeSharedPtr>
  void ProcessNode(NonTerminalTypeSharedPtr node);
  std::set<Token> recorded_elements_;
};

class FocusedElementCollectorLogic : public VisitorProcessLogic {
 public:
  explicit FocusedElementCollectorLogic(OptionBindingRecorder *recorder_ptr);
  std::set<Token> GetFocusedElements();

  template <typename TerminalTypeSharedPtr>
  void ProcessNode(TerminalTypeSharedPtr node);

 private:
  OptionBindingRecorder *recorder_ptr_;
  std::set<Token> operand_candidates_;
};

class OneOrMoreMarkedElementCollectorLogic : public VisitorProcessLogic {
 public:
  explicit OneOrMoreMarkedElementCollectorLogic(
      OptionBindingRecorder *recorder_ptr);
  std::set<Token> GetOneOrMoreMarkedElements();

  template <typename NonTerminalTypeSharedPtr>
  void ProcessNode(NonTerminalTypeSharedPtr node);

 private:
  OptionBindingRecorder *recorder_ptr_;
  NodeRecorderLogic node_recorder_logic_;
};

class OneOrMoreNodeInsertLogic : public VisitorProcessLogic {
 public:
  OneOrMoreNodeInsertLogic(
    const std::set<Token> &focused_oom_bound_options);

  template <typename TerminalTypeSharedPtr>
  void ProcessNode(TerminalTypeSharedPtr node);

 private:
  const std::set<Token> &focused_oom_bound_options_;
};

class BoundArgumentCleaner : public NodeVisitorInterface {
 public:
  using NodeVisitorInterface::ProcessNode;

  explicit BoundArgumentCleaner(const std::set<Token> &bound_arguments);
  void ProcessNode(Argument::SharedPtr node) override;

 private:
  const std::set<Token> &bound_arguments_;
};

}  // namespace clidoc

namespace clidoc {

template <typename NonTerminalTypeSharedPtr>
void StructureOptimizerLogic::ProcessNode(
    NonTerminalTypeSharedPtr node) {
  ConditionalRemoveChild(node);
  ConditionalRemoveParent(node);
  if (node_is_one_of<NonTerminalTypeSharedPtr, LogicOr, LogicXor>::value) {
    if (node->GetSizeOfChildren() == 1) {
      NodeTypeModifier<LogicAnd>::ChangeNonTerminalType(node);
    }
  }
}

template <typename NodeTypeOfParent, typename NodeTypeOfChild>
bool StructureOptimizerLogic::CanRemoveChild(
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
void StructureOptimizerLogic::ConditionalRemoveChild(
    NonTerminalTypeSharedPtr node) {
  // container for processed elements.
  SharedPtrNodeContainer optimized_children;
  // `child_node` must be a reference, since the value of `child_node` in
  // `node->children_` might change.
  for (auto &child_node : node->children_) {
    child_node->Accept(visitor_ptr_);
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
void StructureOptimizerLogic::ConditionalRemoveParent(
    NonTerminalTypeSharedPtr node) {
  if (node->children_.size() == 0) {
    *node->node_connection.this_iter_ = nullptr;
  }
}

template <typename TargetType>
template <typename NonTerminalTypeSharedPtr>
void TerminalTypeModifierLogic<TargetType>::ProcessNode(
    NonTerminalTypeSharedPtr node) {
  NodeTypeModifier<TargetType>::ChangeTerminalType(node);
}

template <typename NonTerminalTypeSharedPtr>
void NodeRecorderLogic::ProcessNode(
    NonTerminalTypeSharedPtr node) {
  recorded_elements_.insert(node->token_);
}

template <typename TerminalTypeSharedPtr>
void FocusedElementCollectorLogic::ProcessNode(
    TerminalTypeSharedPtr node) {
  if (node_is_one_of<TerminalTypeSharedPtr,
                     PosixOption, GnuOption>::value) {
    if (!recorder_ptr_->OptionIsRecorded(node->token_)) {
      recorder_ptr_->RecordSingleOption(node->token_);
    }
  }
  if (node_is_one_of<TerminalTypeSharedPtr,
                     Argument, Command>::value) {
    operand_candidates_.insert(node->token_);
  }
}

template <typename NonTerminalTypeSharedPtr>
void OneOrMoreMarkedElementCollectorLogic::ProcessNode(
    NonTerminalTypeSharedPtr node) {
  if (node_is<NonTerminalTypeSharedPtr, LogicOneOrMore>::value) {
    auto visitor = GenerateVisitor<TerminalVisitor>(&node_recorder_logic_);
    node->Accept(&visitor);
  } else {
    ApplyVisitorToChildren(node, visitor_ptr_);
  }
}

template <typename TerminalTypeSharedPtr>
void OneOrMoreNodeInsertLogic::ProcessNode(
    TerminalTypeSharedPtr node) {
  if (node_is_not<TerminalTypeSharedPtr,
                  PosixOption, GnuOption>::value) {
    return;
  }
  if (focused_oom_bound_options_.find(node->token_)
      != focused_oom_bound_options_.end()) {
    auto logic_one_or_more = LogicOneOrMore::Init();
    node->node_connection.ReplacedWith(logic_one_or_more);
    logic_one_or_more->AddChild(node);
  }
}

}  // namespace clidoc

#endif  //  SRC_AST_PROCESS_LOGIC_H_
