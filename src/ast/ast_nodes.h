#ifndef SRC_AST_AST_NODES_H_
#define SRC_AST_AST_NODES_H_

#include <stdexcept>
#include <cstddef>
#include <sstream>
#include <string>

#include "ast/ast_node_interface.h"
#include "shared/smart_ptr_interface.h"

namespace clidoc {

// Template for terminal types.
template <TerminalType T>
class Terminal : public NodeInterface,
                 public SmartPtrInterface<Terminal<T>> {
 public:
  explicit Terminal(const Token &token);
  explicit Terminal(const std::string &value);

  std::string GetID() override;
  std::size_t GetSizeOfChildren() override;
  std::string ToString() override;
  std::string ToString(const int &indent) override;
  void Accept(NodeVistorInterface *visitor_ptr) override;

  const Token token_;
};

// Template for non-terminal types.
template <NonTerminalType T>
class NonTerminal : public NodeInterface,
                    public SmartPtrInterface<NonTerminal<T>> {
 public:
  std::string GetID() override;
  std::size_t GetSizeOfChildren() override;
  std::string ToString() override;
  std::string ToString(const int &indent) override;
  void Accept(NodeVistorInterface *visitor_ptr) override;

  void AddChild(SharedPtrNode node);
  // Container of symbols.
  SharedPtrNodeContainer children_;
};

// Terminal classes.
using KDoubleHyphen  = Terminal<TerminalType::K_DOUBLE_HYPHEN>;
using KOptions       = Terminal<TerminalType::K_OPTIONS>;

using PosixOption    = Terminal<TerminalType::POSIX_OPTION>;
using GroupedOptions = Terminal<TerminalType::GROUPED_OPTIONS>;
using GnuOption      = Terminal<TerminalType::GNU_OPTION>;
using Argument       = Terminal<TerminalType::ARGUMENT>;
using Command        = Terminal<TerminalType::COMMAND>;

// Non-terminal classes.
using Doc            = NonTerminal<NonTerminalType::DOC>;
using LogicAnd       = NonTerminal<NonTerminalType::LOGIC_AND>;
using LogicOr        = NonTerminal<NonTerminalType::LOGIC_OR>;
using LogicXor       = NonTerminal<NonTerminalType::LOGIC_XOR>;
using LogicOptional  = NonTerminal<NonTerminalType::LOGIC_OPTIONAL>;
using LogicOneOrMore = NonTerminal<NonTerminalType::LOGIC_ONEORMORE>;

}  // namespace clidoc

// definition of visitor.
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
  virtual void ProcessNode(typename Terminal<T>::SharedPtr node) {
    // empty.
  }
};

template <NonTerminalType T, NonTerminalType... RestType>
struct NonTerminalVistorInterface<T, RestType...>
    : public NonTerminalVistorInterface<RestType...> {
  // same wth TerminalVistorInterface.
  using NonTerminalVistorInterface<RestType...>::ProcessNode;

  // interface for NonTerminal<T>::SharedPtr.
  virtual void ProcessNode(typename NonTerminal<T>::SharedPtr node) {
    // Apply vistor to children.
    auto cache_children = node->children_;
    for (auto child_ptr : cache_children) {
      child_ptr->Accept(dynamic_cast<NodeVistorInterface *>(this));
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
  NonTerminalType::LOGIC_OR,
  NonTerminalType::LOGIC_XOR,
  NonTerminalType::LOGIC_OPTIONAL,
  NonTerminalType::LOGIC_ONEORMORE>;

struct NodeVistorInterface : public ConcreteTerminalVistorInterface,
                             public ConcreteNonTerminalVistorInterface {
  using ConcreteTerminalVistorInterface::ProcessNode;
  using ConcreteNonTerminalVistorInterface::ProcessNode;
};

}  // namespace clidoc

// definition of nodes.
namespace clidoc {

template <TerminalType T>
inline Terminal<T>::Terminal(const Token &token)
    : token_(token) {
  if (token.type() != T) {
    throw std::logic_error("Terminal Type Not Match.");
  }
}

template <TerminalType T>
inline Terminal<T>::Terminal(const std::string &value)
    : token_(T, value) { /* empty */ }

template <TerminalType T>
std::string Terminal<T>::GetID() {
  std::string token_value = token_.has_value() ? token_.value() : "NoValue";
  return kTermianlClassName.at(T) + "[" + token_value + "]";
}

template <TerminalType T>
std::size_t Terminal<T>::GetSizeOfChildren() {
  return 0;
}

template <TerminalType T>
std::string Terminal<T>::ToString() {
  return GetID();
}

template <TerminalType T>
std::string Terminal<T>::ToString(const int &indent) {
  std::ostringstream strm;
  strm << GetIndent(indent) << GetID() << std::endl;
  return strm.str();
}

template <TerminalType T>
void Terminal<T>::Accept(NodeVistorInterface *visitor_ptr) {
  visitor_ptr->ProcessNode(this->shared_from_this());
}

template <NonTerminalType T>
std::string NonTerminal<T>::GetID() {
  return kNonTermianlClassName.at(T);
}

template <NonTerminalType T>
std::size_t NonTerminal<T>::GetSizeOfChildren() {
  return children_.size();
}

template <NonTerminalType T>
std::string NonTerminal<T>::ToString() {
  std::ostringstream strm;
  strm << GetID() << "(";
  std::size_t index = 0;
  for (auto ptr : children_) {
    strm << ptr->ToString();
    if (index != children_.size() - 1) {
      strm << ", ";
      ++index;
    }
  }
  strm << ")";
  return strm.str();
}

template <NonTerminalType T>
std::string NonTerminal<T>::ToString(const int &indent) {
  std::ostringstream strm;
  auto prefix = GetIndent(indent);
  strm << prefix << GetID() << "(" << std::endl;
  for (auto ptr : children_) {
    strm << ptr->ToString(indent + 1);
  }
  strm << prefix << ")" << std::endl;
  return strm.str();
}

template <NonTerminalType T>
void NonTerminal<T>::Accept(NodeVistorInterface *visitor_ptr) {
  visitor_ptr->ProcessNode(this->shared_from_this());
}

template <NonTerminalType T>
void NonTerminal<T>::AddChild(SharedPtrNode node) {
  children_.push_back(node);
  // make connection.
  node->node_connection.ConnectParent(this->shared_from_this());
}

}  // namespace clidoc

#endif  // SRC_AST_AST_NODES_H_
