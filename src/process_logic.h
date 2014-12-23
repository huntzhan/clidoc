#ifndef SRC_PROCESS_LOGIC_H_
#define SRC_PROCESS_LOGIC_H_

#include <memory>
#include <stack>
#include "utils.h"

namespace clidoc {

struct StructureOptimizer : public NodeVistorInterface {
  // Since "The nested-name-specifier (everything to the left of the scope
  // resolution operator ::) of a type that was specified using a
  // qualified-id." is one of "Non-deduced contexts",
  // `NonTerminalType<T>::SharedPtr` could not be the function parameter.
  template <NonTerminalType T>
  void RemoveDuplicatedNodes(std::shared_ptr<NonTerminal<T>> node_ptr);

  void ProcessNode(Doc::SharedPtr node_ptr) override {
    RemoveDuplicatedNodes(node_ptr);
  }
  void ProcessNode(LogicAnd::SharedPtr node_ptr) override {
    RemoveDuplicatedNodes(node_ptr);
  }
  void ProcessNode(LogicXor::SharedPtr node_ptr) override {
    RemoveDuplicatedNodes(node_ptr);
  }

  VecSharedPtrNode children_of_child;
};

template <NonTerminalType T>
void StructureOptimizer::RemoveDuplicatedNodes(
    std::shared_ptr<NonTerminal<T>> node_ptr) {
  VecSharedPtrNode optimized_children;
  for (auto child_ptr : node_ptr->children_) {
    child_ptr->Accept(this);
    if (child_ptr->GetID() == node_ptr->GetID()) {
      // same non-terminal nodes.
      optimized_children.insert(
          optimized_children.end(),
          children_of_child.begin(), children_of_child.end());
    } else {
      optimized_children.push_back(child_ptr);
    }
  }
  // make a copy of children of current node, which are accessiable to the
  // parent of current node.
  node_ptr->children_ = optimized_children;
  children_of_child = optimized_children;
}

}  // namespace clidoc

#endif  //  SRC_PROCESS_LOGIC_H_
