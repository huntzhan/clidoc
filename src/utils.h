#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <memory>
#include <string>
#include <vector>
#include "generated_parser.h"

namespace clidoc {

class Token {
 public:
  // facilitate usage.
  using Type = BisonGeneratedParser::token_type;
  using TypeID = BisonGeneratedParser::token;

  // constructors.
  Token(Type type_id)
      : has_value_(false), type_id_(type_id) { /* empty */ }
  Token(Type type_id, const std::string &value)
      : has_value_(true), type_id_(type_id), value_(value) { /* empty */ }

  bool has_value() const { return has_value_; }
  std::string value() const { return value_; }
  Type type_id() const { return type_id_; }

 private:
  const bool has_value_;
  const Type type_id_;
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
};

using SharedPtrNode = std::shared_ptr<NodeInterface>;
using VecSharedPtrNode = std::vector<SharedPtrNode>;

// Template for terminal types.
template <Token::Type T>
class Terminal : public NodeInterface {
 public:
  using SharedPtr = std::shared_ptr<Terminal<T>>;

  Terminal(const Token &token) : token_(token) { /* empty */ }
  bool ProcessToken(TokenInProcessCollection *token_collection) override;

  const Token token_;
};

// Defines non-terminal types.
enum class NonTerminalType {
  USAGE_SECTION,
  UTILITIES,
  SINGLE_UTILITY,
  OR_EXPRS,
  SEQS,
  SINGLE_SEQ,
  ATOM,
  GNU_OPTION_UNIT,
  POSIX_OPTION_UNIT,
  OPTIONS_SECTION,
  DESCRIPTIONS,
  SINGLE_DESCRIPTION,
  BINDINGS,
  SINGLE_BINDING,
  DEFAULT_VALUE,
  COMMENTS,
  SINGLE_COMMENT
};

// Template for non-terminal types.
template <NonTerminalType T>
class NonTerminal : public NodeInterface {
 public:
  using SharedPtr = std::shared_ptr<NonTerminal<T>>;

  bool ProcessToken(TokenInProcessCollection *token_collection) override;

  // Container of symbols.
  VecSharedPtrNode children_;
};

// Terminal classes.
using T_OptionArguement    = Terminal<Token::TypeID::OPTION_ARGUEMENT>;
using T_GnuOption          = Terminal<Token::TypeID::GNU_OPTION>;
using T_Operand            = Terminal<Token::TypeID::OPERAND>;
using T_Comment            = Terminal<Token::TypeID::COMMENT>;
using T_OptionDefaultValue = Terminal<Token::TypeID::OPTION_DEFAULT_VALUE>;
using T_PosixOption        = Terminal<Token::TypeID::POSIX_OPTION>;
using T_GroupedOptions     = Terminal<Token::TypeID::GROUPED_OPTIONS>;

// Non-terminal classes.
using UsageSection         = NonTerminal<NonTerminalType::USAGE_SECTION>;
using Utilities            = NonTerminal<NonTerminalType::UTILITIES>;
using SingleUtility        = NonTerminal<NonTerminalType::SINGLE_UTILITY>;
using OrExprs              = NonTerminal<NonTerminalType::OR_EXPRS>;
using Seqs                 = NonTerminal<NonTerminalType::SEQS>;
using SingleSeq            = NonTerminal<NonTerminalType::SINGLE_SEQ>;
using Atom                 = NonTerminal<NonTerminalType::ATOM>;
using GnuOptionUnit        = NonTerminal<NonTerminalType::GNU_OPTION_UNIT>;
using PosixOptionUnit      = NonTerminal<NonTerminalType::POSIX_OPTION_UNIT>;
using OptionsSection       = NonTerminal<NonTerminalType::OPTIONS_SECTION>;
using Descriptions         = NonTerminal<NonTerminalType::DESCRIPTIONS>;
using SingleDescription    = NonTerminal<NonTerminalType::SINGLE_DESCRIPTION>;
using Bindings             = NonTerminal<NonTerminalType::BINDINGS>;
using SingleBinding        = NonTerminal<NonTerminalType::SINGLE_BINDING>;
using DefaultValue         = NonTerminal<NonTerminalType::DEFAULT_VALUE>;
using Comments             = NonTerminal<NonTerminalType::COMMENTS>;
using SingleComment        = NonTerminal<NonTerminalType::SINGLE_COMMENT>;

}  // namespace clidoc 

#include "utils-inl.h"

#endif
