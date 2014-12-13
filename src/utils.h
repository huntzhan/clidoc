#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <string>
#include "generated_parser.h"

namespace clidoc {

class Token {
 public:
  Token(BisonGeneratedParser::token_type type)
      : has_value_(false), type_(type) { /* empty */ }
  Token(BisonGeneratedParser::token_type type, const std::string &value)
      : has_value_(false), type_(type), value_(value) { /* empty */ }

  std::string value() const {
    return value_;
  }
  virtual BisonGeneratedParser::token_type type() const {
    return type_;
  }

 private:
  const bool has_value_;
  const BisonGeneratedParser::token_type type_;
  const std::string value_;
};

}  // namespace clidoc 
#endif
