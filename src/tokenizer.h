#ifndef SRC_TOKENIZER_H_
#define SRC_TOKENIZER_H_

#include <vector>
#include <memory>

#include "generated_scanner.h"
#include "utils.h"

namespace clidoc {
namespace tokenizer {

// facilitate usage.
using Type = BisonGeneratedParser::token_type;
using TypeID = BisonGeneratedParser::token;
TerminalType ToTerminalType(const Type &type_id);

// encapsulated operations.
bool TokenHasValue(const TerminalType &type);
std::vector<Token> FromString(const std::string &text);

}  // namespace clidoc::tokenizer
}  // namespace clidoc 
#endif
