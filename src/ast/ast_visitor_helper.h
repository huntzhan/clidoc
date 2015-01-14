#ifndef SRC_AST_VISITOR_HELPER_H_
#define SRC_AST_VISITOR_HELPER_H_

#include <type_traits>

#include "ast/ast_node_interface.h"
#include "ast/ast_nodes.h"

namespace clidoc {

template <typename NodeSharedPtr, typename NodeType>
struct node_is {
  static const bool value = std::is_same<
      typename NodeSharedPtr::element_type,
      NodeType>::value;
};

template <typename... Types>
struct node_is_not_;

template <typename NodeSharedPtr, typename NodeType>
struct node_is_not_<NodeSharedPtr, NodeType> {
  using type = typename std::conditional<
      node_is<NodeSharedPtr, NodeType>::value,
      std::false_type,
      std::true_type>::type;
};

template <typename NodeSharedPtr, typename NodeType, typename... RemainTypes>
struct node_is_not_<NodeSharedPtr, NodeType, RemainTypes...> {
  using type = typename std::conditional<
      node_is<NodeSharedPtr, NodeType>::value,
      std::false_type,
      typename node_is_not_<NodeSharedPtr, RemainTypes...>::type>::type;
};

template <typename NodeSharedPtr, typename... NodeTypes>
struct node_is_not {
  static const bool value = std::is_same<
      typename node_is_not_<NodeSharedPtr, NodeTypes...>::type,
      std::true_type>::value;
};

template <typename NodeSharedPtr, typename... NodeTypes>
struct node_is_one_of {
  static const bool value = !node_is_not<NodeSharedPtr, NodeTypes...>::value;
};


template <typename Callable>
struct NodeTypeModifier {
  // Could be invoked other visitors.
  template <typename TerminalTypeSharedPtr>
  static void ChangeTerminalType(TerminalTypeSharedPtr node);
  template <typename NonTerminalTypeSharedPtr>
  static void ChangeNonTerminalType(NonTerminalTypeSharedPtr node);
};

struct VisitorProcessLogic {
  NodeVisitorInterface *visitor_ptr_;
};

template <typename Callable>
class TerminalVisitor : public NodeVisitorInterface {
 public:
  using NodeVisitorInterface::ProcessNode;

  explicit TerminalVisitor(Callable *callable_ptr);

  void ProcessNode(KDoubleHyphen::SharedPtr node) override;
  void ProcessNode(KOptions::SharedPtr node) override;
  void ProcessNode(PosixOption::SharedPtr node) override;
  void ProcessNode(GroupedOptions::SharedPtr node) override;
  void ProcessNode(GnuOption::SharedPtr node) override;
  void ProcessNode(Argument::SharedPtr node) override;
  void ProcessNode(Command::SharedPtr node) override;

 private:
  Callable *callable_ptr_;
};

template <typename Callable>
class NonTerminalVisitor : public NodeVisitorInterface {
 public:
  using NodeVisitorInterface::ProcessNode;

  explicit NonTerminalVisitor(Callable *callable_ptr);

  void ProcessNode(LogicAnd::SharedPtr node) override;
  void ProcessNode(LogicXor::SharedPtr node) override;
  void ProcessNode(LogicOr::SharedPtr node) override;
  void ProcessNode(LogicOptional::SharedPtr node) override;
  void ProcessNode(LogicOneOrMore::SharedPtr node) override;

 private:
  Callable *callable_ptr_;
};

template <template <typename Callable> class Visitor, typename Callable>
Visitor<Callable> GenerateVisitor(Callable *callable_ptr) {
  return Visitor<Callable>(callable_ptr);
}

template <typename NonTerminalTypeSharedPtr>
void ApplyVisitorToChildren(
    NonTerminalTypeSharedPtr node,
    NodeVisitorInterface *visitor_ptr) {
  auto cache_children = node->children_;
  for (auto child_node : cache_children) {
    child_node->Accept(visitor_ptr);
  }
}

}  // namespace clidoc

namespace clidoc {

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

template <typename Callable>
TerminalVisitor<Callable>::TerminalVisitor(Callable *callable_ptr)
    : callable_ptr_(callable_ptr) {
  callable_ptr_->visitor_ptr_ = this;
}

template <typename Callable>
void TerminalVisitor<Callable>::ProcessNode(
    KDoubleHyphen::SharedPtr node) {
  callable_ptr_->ProcessNode(node);
}

template <typename Callable>
void TerminalVisitor<Callable>::ProcessNode(
    KOptions::SharedPtr node) {
  callable_ptr_->ProcessNode(node);
}

template <typename Callable>
void TerminalVisitor<Callable>::ProcessNode(
    PosixOption::SharedPtr node) {
  callable_ptr_->ProcessNode(node);
}

template <typename Callable>
void TerminalVisitor<Callable>::ProcessNode(
    GroupedOptions::SharedPtr node) {
  callable_ptr_->ProcessNode(node);
}

template <typename Callable>
void TerminalVisitor<Callable>::ProcessNode(
    GnuOption::SharedPtr node) {
  callable_ptr_->ProcessNode(node);
}

template <typename Callable>
void TerminalVisitor<Callable>::ProcessNode(
    Argument::SharedPtr node) {
  callable_ptr_->ProcessNode(node);
}

template <typename Callable>
void TerminalVisitor<Callable>::ProcessNode(
    Command::SharedPtr node) {
  callable_ptr_->ProcessNode(node);
}

template <typename Callable>
NonTerminalVisitor<Callable>::NonTerminalVisitor(Callable *callable_ptr)
    : callable_ptr_(callable_ptr) {
  callable_ptr_->visitor_ptr_ = this;
}

template <typename Callable>
void NonTerminalVisitor<Callable>::ProcessNode(
    LogicAnd::SharedPtr node) {
  callable_ptr_->ProcessNode(node);
}

template <typename Callable>
void NonTerminalVisitor<Callable>::ProcessNode(
    LogicXor::SharedPtr node) {
  callable_ptr_->ProcessNode(node);
}

template <typename Callable>
void NonTerminalVisitor<Callable>::ProcessNode(
    LogicOr::SharedPtr node) {
  callable_ptr_->ProcessNode(node);
}

template <typename Callable>
void NonTerminalVisitor<Callable>::ProcessNode(
    LogicOptional::SharedPtr node) {
  callable_ptr_->ProcessNode(node);
}

template <typename Callable>
void NonTerminalVisitor<Callable>::ProcessNode(
    LogicOneOrMore::SharedPtr node) {
  callable_ptr_->ProcessNode(node);
}

}  // namespace clidoc

#endif  // SRC_AST_VISITOR_HELPER_H_
