#ifndef INCLUDE_CLIDOC_AST_PROCESS_LOGIC_H_
#define INCLUDE_CLIDOC_AST_PROCESS_LOGIC_H_

#include <iterator>
#include <set>

#include "clidoc/ast/ast_nodes.h"
#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/ast/ast_visitor_helper.h"
#include "clidoc/ast/option_record.h"
#include "clidoc/ast/parser_proxy.h"

namespace clidoc {

class StructureOptimizerLogic : public VisitorProcessLogic {
 public:
  template <NonTerminalType type>
  void ProcessNode(NonTerminalTypeSharedPtr<type> node);

 private:
  template <NonTerminalType parent_type, typename NodeType>
  bool CanRemoveChild(NonTerminalTypeSharedPtr<parent_type> parent_node,
                      NodeType child_node);

  template <NonTerminalType type>
  bool ConditionalRemoveChild(NonTerminalTypeSharedPtr<type> node);

  template <NonTerminalType type>
  void ConditionalRemoveParent(NonTerminalTypeSharedPtr<type> node);

  SharedPtrNodeContainer children_of_child_;
};

template <typename TargetType>
class TerminalTypeModifierLogic : public VisitorProcessLogic {
 public:
  template <TerminalType type>
  void ProcessNode(TerminalTypeSharedPtr<type> node);
};

class DoubleHyphenHandlerLogic : public VisitorProcessLogic {
 public:
  // Premise: operands after `--` are place at the same level with `--` in AST.
  void ProcessNode(KDoubleHyphen::SharedPtr double_hyphen_node);
};

class AmbiguityHandlerLogic : public VisitorProcessLogic {
 public:
  explicit AmbiguityHandlerLogic(OptionBindingRecorder *recorder_ptr);
  void ProcessNode(GroupedOptions::SharedPtr grouped_options_node);

 private:
  OptionBindingRecorder *recorder_ptr_;
};

class NodeRecorderLogic : public VisitorProcessLogic {
 public:
  template <TerminalType type>
  void ProcessNode(TerminalTypeSharedPtr<type> node);
  std::set<Token> recorded_elements_;
};

class FocusedElementCollectorLogic : public VisitorProcessLogic {
 public:
  explicit FocusedElementCollectorLogic(OptionBindingRecorder *recorder_ptr);
  std::set<Token> GetFocusedElements();

  template <TerminalType type>
  void ProcessNode(TerminalTypeSharedPtr<type> node);

 private:
  OptionBindingRecorder *recorder_ptr_;
  std::set<Token> operand_candidates_;
};

class OneOrMoreMarkedElementCollectorLogic : public VisitorProcessLogic {
 public:
  explicit OneOrMoreMarkedElementCollectorLogic(
      OptionBindingRecorder *recorder_ptr);
  std::set<Token> GetOneOrMoreMarkedElements();

  void ProcessNode(LogicOneOrMore::SharedPtr node);

 private:
  OptionBindingRecorder *recorder_ptr_;
  NodeRecorderLogic node_recorder_logic_;
};

class OneOrMoreNodeInsertLogic : public VisitorProcessLogic {
 public:
  OneOrMoreNodeInsertLogic(
    const std::set<Token> &focused_oom_bound_options);

  template <TerminalType type>
  void ProcessNode(TerminalTypeSharedPtr<type> node);

 private:
  const std::set<Token> &focused_oom_bound_options_;
};

class BoundArgumentCleanerLogic : public VisitorProcessLogic {
 public:
  explicit BoundArgumentCleanerLogic(const std::set<Token> &bound_arguments);
  void ProcessNode(Argument::SharedPtr node);

 private:
  const std::set<Token> &bound_arguments_;
};

}  // namespace clidoc

namespace clidoc {

template <NonTerminalType type>
void StructureOptimizerLogic::ProcessNode(
    NonTerminalTypeSharedPtr<type> node) {
  // keep running if child been removed.
  while (ConditionalRemoveChild(node)) {/* empty */}
  ConditionalRemoveParent(node);
  if ((type == NonTerminalType::LOGIC_OR || type == NonTerminalType::LOGIC_XOR)
      && GetChildrenSize(node) == 1) {
    NodeTypeModifier<LogicAnd>::ChangeNonTerminalType(node);
  }
}

template <NonTerminalType parent_type, typename NodeType>
bool StructureOptimizerLogic::CanRemoveChild(
    NonTerminalTypeSharedPtr<parent_type> parent_node,
    NodeType child_node) {
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
  // size of child_node->children_ equals to 1, remove the child_node.
  if ((child_is_logic_and || child_is_logic_xor || child_is_logic_or)
      && GetChildrenSize(child_node) == 1) {
    equivalent = true;
  }
  return equivalent;
}

template <NonTerminalType type>
bool StructureOptimizerLogic::ConditionalRemoveChild(
    NonTerminalTypeSharedPtr<type> node) {
  bool removed_children_flag = false;
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
      removed_children_flag = true;
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
  return removed_children_flag;
}

template <NonTerminalType type>
void StructureOptimizerLogic::ConditionalRemoveParent(
    NonTerminalTypeSharedPtr<type> node) {
  if (node->children_.size() == 0) {
    *node->node_connection.this_iter_ = nullptr;
  }
}

template <typename TargetType>
template <TerminalType type>
void TerminalTypeModifierLogic<TargetType>::ProcessNode(
    TerminalTypeSharedPtr<type> node) {
  NodeTypeModifier<TargetType>::ChangeTerminalType(node);
}

template <TerminalType type>
void NodeRecorderLogic::ProcessNode(
    TerminalTypeSharedPtr<type> node) {
  recorded_elements_.insert(node->token_);
}

template <TerminalType type>
void FocusedElementCollectorLogic::ProcessNode(
    TerminalTypeSharedPtr<type> node) {
  if ((type == TerminalType::POSIX_OPTION || type == TerminalType::GNU_OPTION)
      && !recorder_ptr_->OptionIsRecorded(node->token_)) {
    recorder_ptr_->RecordSingleOption(node->token_);
  }
  if (type == TerminalType::ARGUMENT || type == TerminalType::COMMAND) {
    operand_candidates_.insert(node->token_);
  }
}

template <TerminalType type>
void OneOrMoreNodeInsertLogic::ProcessNode(
    TerminalTypeSharedPtr<type> node) {
  if (type != TerminalType::POSIX_OPTION && type != TerminalType::GNU_OPTION) {
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

#endif  //  INCLUDE_CLIDOC_AST_PROCESS_LOGIC_H_
