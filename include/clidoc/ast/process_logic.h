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

template <NonTerminalType ParentType>
class ChildRemovabilityChecker : public VisitorProcessLogic {
 public:
  explicit ChildRemovabilityChecker(
      NonTerminalTypeSharedPtr<ParentType> parent_node);

  template <NonTerminalType ChildType>
  void ProcessNode(NonTerminalTypeSharedPtr<ChildType> child_node);

  bool child_is_removable() const;

 private:
  NonTerminalTypeSharedPtr<ParentType> parent_node_;
  bool child_is_removable_ = false;
};

class StructureOptimizerLogic : public VisitorProcessLogic {
 public:
  template <NonTerminalType Type>
  void ProcessNode(NonTerminalTypeSharedPtr<Type> node);

 private:
  template <NonTerminalType Type>
  bool ConditionalRemoveChild(NonTerminalTypeSharedPtr<Type> node);

  template <NonTerminalType Type>
  void ConditionalRemoveParent(NonTerminalTypeSharedPtr<Type> node);

  SharedPtrNodeContainer children_of_child_;
};

template <typename TargetType>
class TerminalTypeModifierLogic : public VisitorProcessLogic {
 public:
  template <TerminalType Type>
  void ProcessNode(TerminalTypeSharedPtr<Type> node);
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
  template <TerminalType Type>
  void ProcessNode(TerminalTypeSharedPtr<Type> node);
  std::set<Token> recorded_elements_;
};

class FocusedElementCollectorLogic : public VisitorProcessLogic {
 public:
  explicit FocusedElementCollectorLogic(OptionBindingRecorder *recorder_ptr);
  std::set<Token> GetFocusedElements();

  template <TerminalType Type>
  void ProcessNode(TerminalTypeSharedPtr<Type> node);

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

  template <TerminalType Type>
  void ProcessNode(TerminalTypeSharedPtr<Type> node);

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

template <NonTerminalType ParentType>
ChildRemovabilityChecker<ParentType>::ChildRemovabilityChecker(
    NonTerminalTypeSharedPtr<ParentType> parent_node)
    : parent_node_(parent_node) { /* empty */ }

template <NonTerminalType ParentType>
template <NonTerminalType ChildType>
void ChildRemovabilityChecker<ParentType>::ProcessNode(
    NonTerminalTypeSharedPtr<ChildType> child_node) {
  // 1. equivalent if they have the same id.
  if (ParentType == ChildType) {
    child_is_removable_ = true;
  }
  // 2. when child_node is one of (`LogicAnd`, `LogicXor`, `LogicOr`), and
  // size of child_node->children() equals to 1, remove the child_node.
  if ((ChildType == NonTerminalType::LOGIC_AND
       || ChildType == NonTerminalType::LOGIC_XOR
       || ChildType == NonTerminalType::LOGIC_OR)
      && child_node->children().size() == 1) {
    child_is_removable_ = true;
  }
}

template <NonTerminalType ParentType>
bool ChildRemovabilityChecker<ParentType>::child_is_removable() const {
  return child_is_removable_;
}

template <NonTerminalType Type>
void StructureOptimizerLogic::ProcessNode(
    NonTerminalTypeSharedPtr<Type> node) {
  // keep running if child been removed.
  while (ConditionalRemoveChild(node)) {/* empty */}
  ConditionalRemoveParent(node);
  if ((Type == NonTerminalType::LOGIC_OR || Type == NonTerminalType::LOGIC_XOR)
      && node->children().size() == 1) {
    NodeTypeModifier<LogicAnd>::ChangeNonTerminalType(node);
  }
}

template <NonTerminalType Type>
bool StructureOptimizerLogic::ConditionalRemoveChild(
    NonTerminalTypeSharedPtr<Type> node) {
  bool removed_children_flag = false;
  // container for processed elements.
  SharedPtrNodeContainer optimized_children;
  // `child_node` must be a reference, since the value of `child_node` in
  // `node->children_` might change.
  for (auto &child_node : node->children()) {
    child_node->Accept(visitor_ptr_);
    if (!child_node) {
      // child node has been removed.
      continue;
    }

    // check child_node is removable or not.
    ChildRemovabilityChecker<Type> child_removability_checker(node);
    auto visitor = GenerateVisitor<NonTerminalVisitor>(
        &child_removability_checker);
    child_node->Accept(&visitor);

    if (child_removability_checker.child_is_removable()) {
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
  node->ClearChildren();
  for (auto child_node : optimized_children) {
    node->AddChild(child_node);
  }
  children_of_child_ = optimized_children;
  return removed_children_flag;
}

template <NonTerminalType Type>
void StructureOptimizerLogic::ConditionalRemoveParent(
    NonTerminalTypeSharedPtr<Type> node) {
  if (node->children().size() == 0) {
    node->ReplacedWith(nullptr);
  }
}

template <typename TargetType>
template <TerminalType Type>
void TerminalTypeModifierLogic<TargetType>::ProcessNode(
    TerminalTypeSharedPtr<Type> node) {
  NodeTypeModifier<TargetType>::ChangeTerminalType(node);
}

template <TerminalType Type>
void NodeRecorderLogic::ProcessNode(
    TerminalTypeSharedPtr<Type> node) {
  recorded_elements_.insert(node->token());
}

template <TerminalType Type>
void FocusedElementCollectorLogic::ProcessNode(
    TerminalTypeSharedPtr<Type> node) {
  if ((Type == TerminalType::POSIX_OPTION || Type == TerminalType::GNU_OPTION)
      && !recorder_ptr_->OptionIsRecorded(node->token())) {
    recorder_ptr_->RecordSingleOption(node->token());
  }
  if (Type == TerminalType::ARGUMENT || Type == TerminalType::COMMAND) {
    operand_candidates_.insert(node->token());
  }
}

template <TerminalType Type>
void OneOrMoreNodeInsertLogic::ProcessNode(
    TerminalTypeSharedPtr<Type> node) {
  if (Type != TerminalType::POSIX_OPTION && Type != TerminalType::GNU_OPTION) {
    return;
  }
  if (focused_oom_bound_options_.find(node->token())
      != focused_oom_bound_options_.end()) {
    auto logic_one_or_more = LogicOneOrMore::Init();
    node->ReplacedWith(logic_one_or_more);
    logic_one_or_more->AddChild(node);
  }
}

}  // namespace clidoc

#endif  //  INCLUDE_CLIDOC_AST_PROCESS_LOGIC_H_
