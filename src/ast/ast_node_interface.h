#ifndef SRC_AST_AST_NODE_INTERFACE_H_
#define SRC_AST_AST_NODE_INTERFACE_H_

#include <cstddef>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <list>
#include <iterator>

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

// forward declaration for `SharedPtrNode`...
class NodeInterface;
// forward declaration for `NodeConnection::ConnectParent`.
template <NonTerminalType T>
class NonTerminal;

using SharedPtrNode = std::shared_ptr<NodeInterface>;
using WeakPtrNode = std::weak_ptr<NodeInterface>;
using SharedPtrNodeContainer = std::list<SharedPtrNode>;

// Record the binding of parent and child.
struct NodeConnection {
  // `ConnectParent` make connection with parent node.
  // by manually set.
  void ConnectParent(
      SharedPtrNodeContainer::iterator other_this_iter,
      SharedPtrNodeContainer *other_children_of_parent_ptr);
  // by copying the setting of other.
  void ConnectParent(NodeConnection *other);
  // by connect to the last child of parent.
  template <typename NonTerminalTypeSharedPtr>
  void ConnectParent(NonTerminalTypeSharedPtr parent_node);

  // Replace this node with another node.
  template <typename NodeTypeSharedPtr>
  void ReplacedWith(NodeTypeSharedPtr node_ptr);

  SharedPtrNodeContainer::iterator this_iter_;
  SharedPtrNodeContainer *children_of_parent_ptr_ = nullptr;
};

struct NodeVisitorInterface;

// Interface for symbols in parsing tree.
class NodeInterface {
 public:
  virtual ~NodeInterface() = default;

  // inline member helps generating indented prefix.
  std::string GetIndent(const int &indent) const;
  // get the string identify CURRENT node.
  virtual std::string GetID() = 0;
  // get the size of children. terminal type would return 0.
  virtual std::size_t GetSizeOfChildren() = 0;
  // encode the tree structure rooted by current node as string.
  virtual std::string ToString() = 0;
  // indented version of ToString().
  virtual std::string ToString(const int &indent) = 0;
  // Apply visitor design pattern!
  virtual void Accept(NodeVisitorInterface *visitor_ptr) = 0;
  // Connection of nodes in AST.
  NodeConnection node_connection;
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

}  // namespace clidoc

namespace clidoc {

inline void NodeConnection::ConnectParent(
    SharedPtrNodeContainer::iterator other_this_iter,
    SharedPtrNodeContainer *other_children_of_parent_ptr) {
  this_iter_ = other_this_iter;
  children_of_parent_ptr_ = other_children_of_parent_ptr;
}


inline void NodeConnection::ConnectParent(NodeConnection *other) {
  ConnectParent(other->this_iter_, other->children_of_parent_ptr_);
}

template <typename NonTerminalTypeSharedPtr>
void NodeConnection::ConnectParent(NonTerminalTypeSharedPtr parent_node) {
  ConnectParent(std::prev(parent_node->children_.end()),
                &parent_node->children_);
}

template <typename NodeTypeSharedPtr>
void NodeConnection::ReplacedWith(NodeTypeSharedPtr node_ptr) {
  *this_iter_ = node_ptr;
  node_ptr->node_connection.ConnectParent(this);
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

inline Token::Token(TerminalType type)
    : has_value_(false), type_(type) { /* empty */ }

inline Token::Token(TerminalType type, const std::string &value)
    : has_value_(true), type_(type), value_(value) { /* empty */ }

// support std::map.
inline bool Token::operator<(const Token &other) const {
  return value_ < other.value_;
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
  return type_ == TerminalType::OTHER
         || (has_value_ && value_.empty());
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

}  // namespace clidoc

#endif  // SRC_AST_AST_NODE_INTERFACE_H_
