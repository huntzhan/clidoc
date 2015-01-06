#ifndef SRC_TOKEN_PROXY_H_
#define SRC_TOKEN_PROXY_H_

#include <string>

#include "ast/generated_parser.h"
#include "ast/ast_node_interface.h"

namespace clidoc {

// facilitate usage.
using Type = BisonGeneratedParser::token_type;
using TypeID = BisonGeneratedParser::token;

TerminalType ToTerminalType(const Type &type_id);
bool TokenHasValue(const TerminalType &type);

Token InitToken(const TerminalType &terminal_type, const std::string &value);
Token InitToken(const Type &type_id, const std::string &value);

}  // namespace clidoc
#endif  // SRC_TOKEN_PROXY_H_
