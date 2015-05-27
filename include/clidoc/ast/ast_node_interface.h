#ifndef INCLUDE_CLIDOC_AST_AST_NODE_INTERFACE_H_
#define INCLUDE_CLIDOC_AST_AST_NODE_INTERFACE_H_

#include <cstddef>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <string>

namespace clidoc {

// Defines terminal types.
enum class TerminalType {
  K_OPTIONS,
  K_DOUBLE_HYPHEN,

  POSIX_OPTION,
  GROUPED_OPTIONS,
  GNU_OPTION,
  ARGUMENT,
  DEFAULT_VALUE,
  COMMAND,

  GENERAL_ELEMENT,

  // for terminals that would not be instaniation.
  OTHER,
};

// Defines non-terminal types.
enum class NonTerminalType {
  // logical.
  LOGIC_AND,
  LOGIC_OR,
  LOGIC_XOR,
  LOGIC_OPTIONAL,
  LOGIC_ONEORMORE,
  LOGIC_EMPTY,
  // start node.
  DOC,
};

const std::map<TerminalType, std::string> kTermianlClassName = {
  {TerminalType::K_OPTIONS,       "KOptions"},
  {TerminalType::K_DOUBLE_HYPHEN, "KDoubleHyphen"},

  {TerminalType::POSIX_OPTION,    "PosixOption"},
  {TerminalType::GROUPED_OPTIONS, "GroupedOptions"},
  {TerminalType::GNU_OPTION,      "GnuOption"},
  {TerminalType::ARGUMENT,        "Argument"},
  {TerminalType::COMMAND,         "Command"},

  {TerminalType::GENERAL_ELEMENT, "GeneralElement"},
};

const std::map<NonTerminalType, std::string> kNonTermianlClassName = {
  {NonTerminalType::DOC,             "Doc"},
  {NonTerminalType::LOGIC_AND,       "LogicAnd"},
  {NonTerminalType::LOGIC_OR,        "LogicOr"},
  {NonTerminalType::LOGIC_XOR,       "LogicXor"},
  {NonTerminalType::LOGIC_OPTIONAL,  "LogicOptional"},
  {NonTerminalType::LOGIC_ONEORMORE, "LogicOneOrMore"},
};

const std::map<TerminalType, std::string> kTermianlValueName = {
  {TerminalType::K_OPTIONS,       "TerminalType::K_OPTIONS"},
  {TerminalType::K_DOUBLE_HYPHEN, "TerminalType::K_DOUBLE_HYPHEN"},

  {TerminalType::POSIX_OPTION,    "TerminalType::POSIX_OPTION"},
  {TerminalType::GROUPED_OPTIONS, "TerminalType::GROUPED_OPTIONS"},
  {TerminalType::GNU_OPTION,      "TerminalType::GNU_OPTION"},
  {TerminalType::ARGUMENT,        "TerminalType::ARGUMENT"},
  {TerminalType::COMMAND,         "TerminalType::COMMAND"},

  {TerminalType::GENERAL_ELEMENT, "TerminalType::GENERAL_ELEMENT"},
};

// Basic element to store data.
class Token {
 public:
  // constructors.
  Token() = default;
  explicit Token(TerminalType type);
  Token(TerminalType type, const std::string &value);

  // support std::map.
  bool operator<(const Token &other) const;
  // support equality test.
  bool operator==(const Token &other) const;
  bool operator!=(const Token &other) const;

  bool IsEmpty() const;
  std::string ToString() const;

  // accessors.
  bool has_value() const;
  TerminalType type() const;
  std::string value() const;
  // mutator.
  void set_type(const TerminalType &type);
  void set_value(const std::string &value);

 private:
  bool has_value_ = false;
  TerminalType type_ = TerminalType::OTHER;
  std::string value_;
};

// forward declaration for `SharedPtrNode`...
class NodeInterface;
// forward declaration for `NodeConnection::ConnectParent`.
class NonTerminalInterface;

using SharedPtrNode = std::shared_ptr<NodeInterface>;
using WeakPtrNode = std::weak_ptr<NodeInterface>;
using SharedPtrNodeContainer = std::list<SharedPtrNode>;
using SharedPtrNonTerminalInterface = std::shared_ptr<NonTerminalInterface>;

// Record the binding of parent and child.
class NodeConnection {
 public:
  void Init(
      SharedPtrNonTerminalInterface parent_node,
      SharedPtrNodeContainer::iterator this_iter);
  void CopyFrom(const NodeConnection &other);
  void ConnectParent(
      SharedPtrNonTerminalInterface parent_node,
      SharedPtrNode child_node);
  void EraseFromAST();

  SharedPtrNodeContainer::iterator this_iter() const;
  SharedPtrNodeContainer::iterator GetEndOfChildrenOfParent() const;

 private:
  SharedPtrNonTerminalInterface parent_node_ = nullptr;
  SharedPtrNodeContainer::iterator this_iter_;
};

struct NodeVisitorInterface;

// Interface for symbols in parsing tree.
class NodeInterface {
 public:
  virtual ~NodeInterface() = default;
  // encode the tree structure rooted by current node as string.
  virtual std::string ToString() const = 0;
  // indented version of ToString().
  virtual std::string ToString(const int &indent) const = 0;
  // Apply visitor design pattern!
  virtual void Accept(NodeVisitorInterface *visitor_ptr) = 0;

  // operations related to node relations.
  void ConnectParent(
      SharedPtrNonTerminalInterface parent_node,
      SharedPtrNode child_node);
  void ReplacedWith(SharedPtrNode node);
  void EraseFromAST();
  SharedPtrNodeContainer::iterator GetThisIter() const;
  SharedPtrNodeContainer::iterator GetEndOfChildrenOfParent() const;

 protected:
  // inline member helps generating indented prefix.
  std::string GetIndent(const int &indent) const;

 private:
  // Connection of nodes in AST.
  NodeConnection node_connection;
};

class TerminalInterface : public NodeInterface {
 public:
  TerminalInterface(
      const TerminalType &type,
      const std::string &value);
  explicit TerminalInterface(const Token &token);

  const Token &token() const;
  std::string TokenValue() const;

 private:
  const Token token_;
};

class NonTerminalInterface : public NodeInterface {
 public:
  const SharedPtrNodeContainer &children() const;
  void ClearChildren();

 private:
  friend class NodeConnection;
  SharedPtrNodeContainer children_;
};

}  // namespace clidoc

namespace clidoc {

inline Token::Token(TerminalType type)
    : has_value_(false), type_(type) { /* empty */ }

inline Token::Token(TerminalType type, const std::string &value)
    : has_value_(true), type_(type), value_(value) { /* empty */ }

// support std::map.
inline bool Token::operator<(const Token &other) const {
  return value_ < other.value_
         || ((value_ == other.value_) && (type_ < other.type_));
}

// support equality test.
inline bool Token::operator==(const Token &other) const {
  return has_value_ == other.has_value()
         && type_ == other.type()
         && value_ == other.value();
}

inline bool Token::operator!=(const Token &other) const {
  return !(*this== other);
}

inline bool Token::IsEmpty() const {
  // is TerminalType::OTHER or has empty value.
  return type_ == TerminalType::OTHER;
}

inline std::string Token::ToString() const {
  return "Token(" + kTermianlValueName.at(type_)
         + ", \"" + value_ + "\")";
}

// accessors.
inline bool Token::has_value() const {
  return has_value_;
}

inline TerminalType Token::type() const {
  return type_;
}

inline std::string Token::value() const {
  return value_;
}

// mutator.
inline void Token::set_type(const TerminalType &type) {
  type_ = type;
}

inline void Token::set_value(const std::string &value) {
  value_ = value;
}

inline void NodeConnection::Init(
    SharedPtrNonTerminalInterface parent_node,
    SharedPtrNodeContainer::iterator this_iter) {
  parent_node_ = parent_node;
  this_iter_ = this_iter;
}

inline void NodeConnection::CopyFrom(const NodeConnection &other) {
  Init(other.parent_node_, other.this_iter_);
}

inline void NodeConnection::ConnectParent(
    SharedPtrNonTerminalInterface parent_node,
    SharedPtrNode child_node) {
  parent_node->children_.push_back(child_node);
  Init(parent_node, std::prev(parent_node->children_.end()));
}

inline void NodeConnection::EraseFromAST() {
  parent_node_->children_.erase(this_iter_);
  this_iter_ = parent_node_->children_.end();
  parent_node_ = nullptr;
}

inline SharedPtrNodeContainer::iterator
NodeConnection::this_iter() const {
  return this_iter_;
}

inline SharedPtrNodeContainer::iterator
NodeConnection::GetEndOfChildrenOfParent() const {
  return parent_node_->children_.end();
}

inline TerminalInterface::TerminalInterface(
    const TerminalType &type,
    const std::string &value)
    : token_(type, value) { /* empty */ }

inline TerminalInterface::TerminalInterface(const Token &token)
    : token_(token) { /* empty */ }

inline const Token &TerminalInterface::token() const {
  return token_;
}

inline std::string TerminalInterface::TokenValue() const {
  return token_.value();
}

inline void NodeInterface::ConnectParent(
    SharedPtrNonTerminalInterface parent_node,
    SharedPtrNode child_node) {
  node_connection.ConnectParent(parent_node, child_node);
}

inline void NodeInterface::ReplacedWith(SharedPtrNode node) {
  auto this_iter = node_connection.this_iter();
  if (node) {
    // replace iterator of children of parent.
    *this_iter = node;
    // copy connection.
    node->node_connection.CopyFrom(node_connection);
  } else {
    *this_iter = nullptr;
  }
}

inline void NodeInterface::EraseFromAST() {
  node_connection.EraseFromAST();
}

inline SharedPtrNodeContainer::iterator
NodeInterface::GetThisIter() const {
  return node_connection.this_iter();
}

inline SharedPtrNodeContainer::iterator
NodeInterface::GetEndOfChildrenOfParent() const {
  return node_connection.GetEndOfChildrenOfParent();
}

// This member function must be marked inline, otherwise a linkage error would
// be raised.
inline std::string NodeInterface::GetIndent(const int &indent) const {
  std::string indent_element = "| ";
  std::ostringstream strm;
  for (int repeat_times = 0; repeat_times < indent; ++repeat_times) {
    strm << indent_element;
  }
  return strm.str();
}

inline const SharedPtrNodeContainer &NonTerminalInterface::children() const {
  return children_;
}

inline void NonTerminalInterface::ClearChildren() {
  children_.clear();
}

}  // namespace clidoc

#endif  // INCLUDE_CLIDOC_AST_AST_NODE_INTERFACE_H_
