
namespace clidoc {

template <TerminalType T>
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
