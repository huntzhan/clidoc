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
  // explicit Terminal(const Token &token) : token_(token) {
  //   if (token.type() != T) { throw std::logic_error("Terminal"); }
  // }
  // explicit Terminal(const std::string &value)
  //     : token_(Token(T, value)) { /* empty */ }
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

  void AddChild(SharedPtrNode node_ptr);
  // Container of symbols.
  VecSharedPtrNode children_;
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

}  // namespace clidoc

#include "ast/ast_nodes_visitor-inl.h"
#include "ast_nodes-inl.h"

#endif  // SRC_AST_AST_NODES_H_
