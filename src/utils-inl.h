
namespace clidoc {

template <Token::Type T>
bool Terminal<T>::ProcessToken(
    TokenInProcessCollection *token_collection) {
  throw "NotImplementedError.";
}

template <NonTerminalType T>
bool NonTerminal<T>::ProcessToken(
    TokenInProcessCollection *token_collection) {
  throw "NotImplementedError.";
}

}  // namespace clidoc
