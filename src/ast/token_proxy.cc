
#include <string>

#include "ast/ast_node_interface.h"
#include "ast/token_proxy.h"

using std::string;

// I know this is ugly. Fix it if you have a better plan.
#define TokenTypeMapping(name) \
case TypeID::name:             \
  return TerminalType::name    \

namespace clidoc {

TerminalType ToTerminalType(const Type &type_id) {
  switch (type_id) {
    TokenTypeMapping(K_OPTIONS);
    TokenTypeMapping(K_DOUBLE_HYPHEN);

    TokenTypeMapping(POSIX_OPTION);
    TokenTypeMapping(GROUPED_OPTIONS);
    TokenTypeMapping(GNU_OPTION);
    TokenTypeMapping(ARGUMENT);
    TokenTypeMapping(DEFAULT_VALUE);
    TokenTypeMapping(COMMAND);

    TokenTypeMapping(GENERAL_ELEMENT);
    default:
      return TerminalType::OTHER;
  }
}

bool TokenHasValue(const TerminalType &type) {
  switch (type) {
    case TerminalType::POSIX_OPTION:
    case TerminalType::GROUPED_OPTIONS:
    case TerminalType::GNU_OPTION:

    case TerminalType::ARGUMENT:
    case TerminalType::DEFAULT_VALUE:
    case TerminalType::COMMAND:

    case TerminalType::GENERAL_ELEMENT:
      return true;
    default:
      return false;
  }
}

Token InitToken(const TerminalType &type, const string &value) {
  if (TokenHasValue(type)) {
    return Token(type, value);
  } else {
    return Token(type);
  }
}

Token InitToken(const Type &type_id, const string &value) {
  auto terminal_type = ToTerminalType(type_id);
  return InitToken(terminal_type, value);
}

}  // namespace clidoc
