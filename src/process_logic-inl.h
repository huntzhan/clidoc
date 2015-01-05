namespace clidoc {

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
          children_of_child_.begin(), children_of_child_.end());
    } else {
      optimized_children.push_back(child_ptr);
    }
  }
  // make a copy of children of current node, which are accessiable to the
  // parent of current node.
  node_ptr->children_ = optimized_children;
  children_of_child_ = optimized_children;
}

}  // namespace clidoc
