#ifndef SRC_PROCESS_LOGIC_H_
#define SRC_PROCESS_LOGIC_H_

#include <memory>
#include <stack>
#include "ast/ast_nodes.h"

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

  VecSharedPtrNode children_of_child_;
};

}  // namespace clidoc

#include "process_logic-inl.h"

#endif  //  SRC_PROCESS_LOGIC_H_
