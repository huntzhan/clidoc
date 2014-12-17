#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <memory>
#include <string>
#include <vector>

namespace clidoc {

// Defines terminal types.
enum class TerminalType {
  OPTION_ARGUEMENT,
  GNU_OPTION,
  OPERAND,
  COMMENT,
  ARGUMENT,
  K_DOUBLE_HYPHEN,
  OPTION_DEFAULT_VALUE,
  POSIX_OPTION,
  GROUPED_OPTIONS,
  // for terminals that would not be instaniation.
  OTHER,
};

// Defines non-terminal types.
enum class NonTerminalType {
  // normal.
  USAGE_SECTION,
  SINGLE_UTILITY,
  SINGLE_SEQ,
  ATOM,
  GNU_OPTION_UNIT,
  POSIX_OPTION_UNIT,
  OPTIONS_SECTION,
  SINGLE_DESCRIPTION,
  SINGLE_BINDING,
  DEFAULT_VALUE,
  SINGLE_COMMENT,
  // logical.
  LOGIX_AND,
  LOGIC_XOR,
  LOGIC_OPTIONAL,
  LOGIC_ONEORMORE,
  LOGIC_EMPTY,
  // start node.
  DOC,
};

class Token {
 public:
  // constructors.
  Token(TerminalType type)
      : has_value_(false), type_(type) { /* empty */ }
  Token(TerminalType type, const std::string &value)
      : has_value_(true), type_(type), value_(value) { /* empty */ }

  bool has_value() const { return has_value_; }
  std::string value() const { return value_; }
  TerminalType type() const { return type_; }

 private:
  const bool has_value_;
  const TerminalType type_;
  const std::string value_;
};

// Contains tokenized arguments during semantic analysis.
class TokenInProcessCollection {
 public:
};

// Interface for symbols in parsing tree.
class NodeInterface {
 public:
  virtual bool ProcessToken(TokenInProcessCollection *token_collection) = 0;
  virtual ~NodeInterface() { /* empty */ }
};

using SharedPtrNode = std::shared_ptr<NodeInterface>;
using VecSharedPtrNode = std::vector<SharedPtrNode>;

template <typename Derived>
class SharedPtrInterface : public NodeInterface {
 public:
  // type alias for shared_ptr.
  using SharedPtr = std::shared_ptr<Derived>;
  // function for init shared_ptr.
  template <typename... Args>
  static SharedPtr Init(Args&&... args);
};

template <typename Derived>
template <typename... Args>
typename SharedPtrInterface<Derived>::SharedPtr
SharedPtrInterface<Derived>::Init(Args&&... args) {
  return std::make_shared<Derived>(args...);
}

// Template for terminal types.
template <TerminalType T>
class Terminal : public SharedPtrInterface<Terminal<T>> {
 public:
  Terminal(const Token &token) : token_(token) { /* empty */ }
  bool ProcessToken(TokenInProcessCollection *token_collection) override;

  const Token token_;
};

// Template for non-terminal types.
template <NonTerminalType T>
class NonTerminal : public SharedPtrInterface<NonTerminal<T>> {
 public:
  using SharedPtr = std::shared_ptr<NonTerminal>;

  bool ProcessToken(TokenInProcessCollection *token_collection) override;

  // Container of symbols.
  VecSharedPtrNode children_;
};

// Terminal classes.
using T_OptionArguement    = Terminal<TerminalType::OPTION_ARGUEMENT>;
using T_GnuOption          = Terminal<TerminalType::GNU_OPTION>;
using T_Operand            = Terminal<TerminalType::OPERAND>;
using T_Comment            = Terminal<TerminalType::COMMENT>;
using T_OptionDefaultValue = Terminal<TerminalType::OPTION_DEFAULT_VALUE>;
using T_PosixOption        = Terminal<TerminalType::POSIX_OPTION>;
using T_GroupedOptions     = Terminal<TerminalType::GROUPED_OPTIONS>;
using T_KDoubleHyphen      = Terminal<TerminalType::K_DOUBLE_HYPHEN>;

// Non-terminal classes.
using UsageSection         = NonTerminal<NonTerminalType::USAGE_SECTION>;
using SingleUtility        = NonTerminal<NonTerminalType::SINGLE_UTILITY>;
using SingleSeq            = NonTerminal<NonTerminalType::SINGLE_SEQ>;
using Atom                 = NonTerminal<NonTerminalType::ATOM>;
using GnuOptionUnit        = NonTerminal<NonTerminalType::GNU_OPTION_UNIT>;
using PosixOptionUnit      = NonTerminal<NonTerminalType::POSIX_OPTION_UNIT>;
using OptionsSection       = NonTerminal<NonTerminalType::OPTIONS_SECTION>;
using SingleDescription    = NonTerminal<NonTerminalType::SINGLE_DESCRIPTION>;
using SingleBinding        = NonTerminal<NonTerminalType::SINGLE_BINDING>;
using DefaultValue         = NonTerminal<NonTerminalType::DEFAULT_VALUE>;
using SingleComment        = NonTerminal<NonTerminalType::SINGLE_COMMENT>;

using LogicAnd             = NonTerminal<NonTerminalType::LOGIX_AND>;
using LogicXor             = NonTerminal<NonTerminalType::LOGIC_XOR>;
using LogicOptional        = NonTerminal<NonTerminalType::LOGIC_OPTIONAL>;
using LogicOneOrMore       = NonTerminal<NonTerminalType::LOGIC_ONEORMORE>;
using LogicEmpty           = NonTerminal<NonTerminalType::LOGIC_EMPTY>;

using Doc = NonTerminal<NonTerminalType::DOC>;

}  // namespace clidoc 

#include "utils-inl.h"

#endif
