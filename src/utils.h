#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "smart_ptr_interface.h"

namespace clidoc {

// Defines terminal types.
enum class TerminalType {
  OPTION_ARGUEMENT,
  GNU_OPTION,
  OPERAND,
  COMMENT,
  ARGUMENT,
  K_OPTIONS,
  K_DOUBLE_HYPHEN,
  OPTION_DEFAULT_VALUE,
  POSIX_OPTION,
  GROUPED_OPTIONS,
  // for terminals that would not be instaniation.
  OTHER,
};

const std::map<TerminalType, std::string> kTermianlClassName = {
  {TerminalType::OPTION_ARGUEMENT,     "OptionArguement"},
  {TerminalType::GNU_OPTION,           "GnuOption"},
  {TerminalType::OPERAND,              "Operand"},
  {TerminalType::COMMENT,              "Comment"},
  {TerminalType::OPTION_DEFAULT_VALUE, "OptionDefaultValue"},
  {TerminalType::POSIX_OPTION,         "PosixOption"},
  {TerminalType::GROUPED_OPTIONS,      "GroupedOptions"},
  {TerminalType::K_DOUBLE_HYPHEN,      "KDoubleHyphen"},
  {TerminalType::K_OPTIONS,            "KOptions"},
};

// Defines non-terminal types.
enum class NonTerminalType {
  // logical.
  LOGIX_AND,
  LOGIC_XOR,
  LOGIC_OPTIONAL,
  LOGIC_ONEORMORE,
  LOGIC_EMPTY,
  // start node.
  DOC,
};

const std::map<NonTerminalType, std::string> kNonTermianlClassName = {
  {NonTerminalType::DOC,             "Doc"},
  {NonTerminalType::LOGIX_AND,       "LogicAnd"},
  {NonTerminalType::LOGIC_XOR,       "LogicXor"},
  {NonTerminalType::LOGIC_OPTIONAL,  "LogicOptional"},
  {NonTerminalType::LOGIC_ONEORMORE, "LogicOneOrMore"},
};

class Token {
 public:
  // constructors.
  Token() = default;
  explicit Token(TerminalType type)
      : has_value_(false), type_(type) { /* empty */ }
  Token(TerminalType type, const std::string &value)
      : has_value_(true), type_(type), value_(value) { /* empty */ }

  // support std::map.
  bool operator<(const Token &other) const {
    return value_ < other.value_;
  }
  // support equality test.
  bool operator==(const Token &other) const {
    return (has_value_ == other.has_value()
            && type_ == other.type()
            && value_ == other.value());
  }
  bool operator!=(const Token &other) const {
    return !(*this== other);
  }

  bool IsEmpty() const { return type_ == TerminalType::OTHER; }
  // accessors.
  bool has_value() const { return has_value_; }
  TerminalType type() const { return type_; }
  std::string value() const { return value_; }
  // mutator.
  void set_value(const std::string &value) { value_ = value; }

 private:
  bool has_value_ = false;
  TerminalType type_ = TerminalType::OTHER;
  std::string value_;
};


// Interface for symbols in parsing tree.
class NodeInterface {
 public:
  virtual ~NodeInterface() { /* empty */ }

  // inline member helps generating indented prefix.
  std::string GetIndent(const int &indent) const;
  // get the string identify CURRENT node.
  virtual std::string GetID() = 0;
  // encode the tree structure rooted by current node as string.
  virtual std::string ToString() = 0;
  // indented version of ToString().
  virtual std::string ToString(const int &indent) = 0;
};

using SharedPtrNode = std::shared_ptr<NodeInterface>;
using WeakPtrNode = std::weak_ptr<NodeInterface>;
using VecSharedPtrNode = std::vector<SharedPtrNode>;

// Template for terminal types.
template <TerminalType T>
class Terminal : public NodeInterface,
                 public SmartPtrInterface<Terminal<T>> {
 public:
  explicit Terminal(const Token &token) : token_(token) { /* empty */ }
  std::string GetID() override;
  std::string ToString() override;
  std::string ToString(const int &indent) override;

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
