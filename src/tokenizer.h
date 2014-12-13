#ifndef SRC_TOKENIZER_H_
#define SRC_TOKENIZER_H_

#include <vector>
#include <memory>

#include "utils.h"
#include "generated_parser.h"

namespace clidoc {

struct Tokenizer {
  static std::vector<Token> FromString(const std::string &text);
  static bool TokenHasValue(const BisonGeneratedParser::token_type &type);
};

}  // namespace clidoc 
#endif
