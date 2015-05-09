#ifndef SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_TOKENIZER_H_
#define SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_TOKENIZER_H_

#include <string>
#include <vector>

namespace clidoc {

// encapsulated operations.
std::vector<Token> FromString(const std::string &text);

}  // namespace clidoc

#endif  // SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_TOKENIZER_H_
