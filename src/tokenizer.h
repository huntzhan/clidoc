#ifndef SRC_TOKENIZER_H_
#define SRC_TOKENIZER_H_

#include <string>
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
Token InitToken(const TerminalType &terminal_type, const std::string &value);
Token InitToken(const Type &type_id, const std::string &value);

// encapsulated operations.
bool TokenHasValue(const TerminalType &type);
std::vector<Token> FromString(const std::string &text);

}  // namespace clidoc::tokenizer
}  // namespace clidoc
#endif  // SRC_TOKENIZER_H_
