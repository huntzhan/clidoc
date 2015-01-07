namespace clidoc {

// classes for visitor.
template <TerminalType... T>
struct TerminalVistorInterface;

template <NonTerminalType... T>
struct NonTerminalVistorInterface;

template <>
struct TerminalVistorInterface<> {
  // should not be call anyway.
  virtual void ProcessNode(TerminalType /*empty*/) {
    throw "NotImplementedError.";
  }
};

template <>
struct NonTerminalVistorInterface<> {
  virtual void ProcessNode(NonTerminalType /*empty*/) {
    throw "NotImplementedError.";
  }
};

template <TerminalType T, TerminalType... RestType>
struct TerminalVistorInterface<T, RestType...>
    : public TerminalVistorInterface<RestType...> {
  // import interfaces.
  using TerminalVistorInterface<RestType...>::ProcessNode;

  // interface for Terminal<T>::SharedPtr.
  virtual void ProcessNode(typename Terminal<T>::SharedPtr node_ptr) {
    // empty.
  }
};

template <NonTerminalType T, NonTerminalType... RestType>
struct NonTerminalVistorInterface<T, RestType...>
    : public NonTerminalVistorInterface<RestType...> {
  // same wth TerminalVistorInterface.
  using NonTerminalVistorInterface<RestType...>::ProcessNode;

  // interface for NonTerminal<T>::SharedPtr.
  virtual void ProcessNode(typename NonTerminal<T>::SharedPtr node_ptr) {
    // Apply vistor to children.
    // Fix it if you have better way to do it.
    for (auto child_ptr : node_ptr->children_) {
      // child_ptr->Accept(dynamic_cast<NodeVistorInterface *>(this));
      // use `static_cast` since RTTI check is unnecessary.
      child_ptr->Accept(static_cast<NodeVistorInterface *>(this));
    }
  }
};

using ConcreteTerminalVistorInterface = TerminalVistorInterface<
  TerminalType::K_DOUBLE_HYPHEN,
  TerminalType::K_OPTIONS,

  TerminalType::POSIX_OPTION,
  TerminalType::GROUPED_OPTIONS,
  TerminalType::GNU_OPTION,
  TerminalType::ARGUMENT,
  TerminalType::DEFAULT_VALUE,
  TerminalType::COMMAND>;


using ConcreteNonTerminalVistorInterface = NonTerminalVistorInterface<
  NonTerminalType::DOC,
  NonTerminalType::LOGIC_AND,
  NonTerminalType::LOGIC_XOR,
  NonTerminalType::LOGIC_OPTIONAL,
  NonTerminalType::LOGIC_ONEORMORE>;

struct NodeVistorInterface : public ConcreteTerminalVistorInterface,
                             public ConcreteNonTerminalVistorInterface {
  using ConcreteTerminalVistorInterface::ProcessNode;
  using ConcreteNonTerminalVistorInterface::ProcessNode;
};

}  // namespace clidoc
