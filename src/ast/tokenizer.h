#ifndef SRC_AST_TOKENIZER_H_
#define SRC_AST_TOKENIZER_H_

#include <string>
#include <vector>

namespace clidoc {

// encapsulated operations.
std::vector<Token> FromString(const std::string &text);

}  // namespace clidoc

#endif  // SRC_AST_TOKENIZER_H_
