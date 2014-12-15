#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <string>
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
  // Defines non-terminal types.
  enum class NodeType {
    EXAMPLE
  };
  virtual bool ProcessToken(TokenInProcessCollection *token_collection) = 0;
};

// Template for terminal types.
template <Token::Type T>
class Terminal : public NodeInterface {
 public:
  bool ProcessToken(TokenInProcessCollection *token_collection) override;
};

// Template for non-terminal types.
template <NodeInterface::NodeType T>
class NonTerminal : public NodeInterface {
 public:
  bool ProcessToken(TokenInProcessCollection *token_collection) override;
};

}  // namespace clidoc 

#include "utils-inl.h"

#endif
