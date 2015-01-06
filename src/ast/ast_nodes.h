#ifndef SRC_AST_AST_NODES_H_
#define SRC_AST_AST_NODES_H_

#include <sstream>
#include <string>
#include <vector>

#include "ast/ast_node_interface.h"
#include "shared/smart_ptr_interface.h"

namespace clidoc {

// Template for terminal types.
template <TerminalType T>
class Terminal : public NodeInterface,
                 public SmartPtrInterface<Terminal<T>> {
 public:
  explicit Terminal(const Token &token) : token_(token) { /* empty */ }
  std::string GetID() override;
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
  std::string ToString() override;
  std::string ToString(const int &indent) override;
  void Accept(NodeVistorInterface *visitor_ptr) override;

  // Container of symbols.
  VecSharedPtrNode children_;
};

// classes for visitor.
template <TerminalType... T>
struct TerminalVistorInterface;

template <NonTerminalType... T>
struct NonTerminalVistorInterface;

template <TerminalType type, TerminalType... rest_type>
struct TerminalVistorInterface<type, rest_type...>
    : public TerminalVistorInterface<rest_type...> {
  // import interfaces.
  using TerminalVistorInterface<rest_type...>::ProcessNode;
  // interface for Terminal<type>::SharedPtr.
  virtual void ProcessNode(typename Terminal<type>::SharedPtr node_ptr) {
    // empty.
  }
};

template <>
struct TerminalVistorInterface<> {
  // should not be call anyway.
  virtual void ProcessNode() {
    throw "NotImplementedError.";
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

template <NonTerminalType type, NonTerminalType... rest_type>
struct NonTerminalVistorInterface<type, rest_type...>
    : public NonTerminalVistorInterface<rest_type...> {
  // same wth TerminalVistorInterface.
  using NonTerminalVistorInterface<rest_type...>::ProcessNode;
  virtual void ProcessNode(typename NonTerminal<type>::SharedPtr node_ptr) {
    // empty.
  }
};

template <>
struct NonTerminalVistorInterface<>
    // Derived from terminal interface.
    : public ConcreteTerminalVistorInterface {
  // import interfaces for terminals.
  using ConcreteTerminalVistorInterface::ProcessNode;
};

using ConcreteNonTerminalVistorInterface = NonTerminalVistorInterface<
  NonTerminalType::DOC,
  NonTerminalType::LOGIC_AND,
  NonTerminalType::LOGIC_XOR,
  NonTerminalType::LOGIC_OPTIONAL,
  NonTerminalType::LOGIC_ONEORMORE>;

struct NodeVistorInterface : public ConcreteNonTerminalVistorInterface {
  // here we go.
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
using LogicXor       = NonTerminal<NonTerminalType::LOGIC_XOR>;
using LogicOptional  = NonTerminal<NonTerminalType::LOGIC_OPTIONAL>;
using LogicOneOrMore = NonTerminal<NonTerminalType::LOGIC_ONEORMORE>;

// For capturing option bindings.
class OptionBinding : public SmartPtrInterface<OptionBinding> {
 public:
  // binding without argument, for synonym options, i.e. "-h, --help".
  explicit OptionBinding(const Token &token_option)
      : token_option_(token_option) { /* empty */ }
  // binding option and option_argument.
  OptionBinding(const Token &token_option,
                const Token &token_optin_argument)
      : token_option_(token_option),
        token_option_argument_(token_optin_argument) { /* empty */ }

  const Token token_option_;
  const Token token_option_argument_ = Token();
};

class OptionBindingContainer
    : public SmartPtrInterface<OptionBindingContainer> {
 public:
  std::vector<OptionBinding::SharedPtr> children_;
};

// For capturing default value of option(s) argument.
class DefaultValue : public SmartPtrInterface<DefaultValue> {
 public:
  DefaultValue() = default;
  explicit DefaultValue(const Token &default_value)
      : default_value_(default_value) {
    // the default value is surrounded by double-quote marks, extract it.
    std::string raw_text = default_value_.value();
    std::string extracted_text(raw_text.begin() + 1, raw_text.end() - 1);
    if (!default_value_.IsEmpty()) {
      default_value_.set_value(extracted_text);
    }
  }

  Token default_value_;
};

}  // namespace clidoc

#include "ast_nodes-inl.h"

#endif  // SRC_AST_AST_NODES_H_
