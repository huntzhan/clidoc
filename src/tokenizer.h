#ifndef SRC_TOKENIZER_H_
#define SRC_TOKENIZER_H_

#include <vector>
#include <memory>

#include "utils.h"

namespace clidoc {

struct Tokenizer {
  // encapsulated operations.
  static std::vector<Token> FromString(const std::string &text);
  static bool TokenHasValue(const Token::Type &type_id);
};

}  // namespace clidoc 
#endif
