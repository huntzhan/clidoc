#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <sstream>
#include <string>
#include <vector>

#include "smart_ptr_interface.h"
#include "node_interface.h"

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
  void Accept(NodeVistorInterface *vistor_ptr) override;

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
  void Accept(NodeVistorInterface *vistor_ptr) override;

  // Container of symbols.
  VecSharedPtrNode children_;
};

// Terminal classes.
using OptionArguement    = Terminal<TerminalType::OPTION_ARGUEMENT>;
using GnuOption          = Terminal<TerminalType::GNU_OPTION>;
using Operand            = Terminal<TerminalType::OPERAND>;
using Comment            = Terminal<TerminalType::COMMENT>;
using OptionDefaultValue = Terminal<TerminalType::OPTION_DEFAULT_VALUE>;
using PosixOption        = Terminal<TerminalType::POSIX_OPTION>;
using GroupedOptions     = Terminal<TerminalType::GROUPED_OPTIONS>;
using KDoubleHyphen      = Terminal<TerminalType::K_DOUBLE_HYPHEN>;
using KOptions           = Terminal<TerminalType::K_OPTIONS>;

// Non-terminal classes.
using Doc                = NonTerminal<NonTerminalType::DOC>;
using LogicAnd           = NonTerminal<NonTerminalType::LOGIX_AND>;
using LogicXor           = NonTerminal<NonTerminalType::LOGIC_XOR>;
using LogicOptional      = NonTerminal<NonTerminalType::LOGIC_OPTIONAL>;
using LogicOneOrMore     = NonTerminal<NonTerminalType::LOGIC_ONEORMORE>;

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

#include "utils-inl.h"

#endif  // SRC_UTILS_H_
