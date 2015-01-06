#ifndef SRC_SHARED_TOKENIZER_H_
#define SRC_SHARED_TOKENIZER_H_

#include <string>
#include <vector>

namespace clidoc {

// encapsulated operations.
std::vector<Token> FromString(const std::string &text);

}  // namespace clidoc

#endif  // SRC_SHARED_TOKENIZER_H_
