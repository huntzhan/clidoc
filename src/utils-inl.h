
namespace clidoc {

template <Token::Type T>
bool Terminal<T>::ProcessToken(
    TokenInProcessCollection *token_collection) {
  throw "NotImplementedError.";
}

template <NodeInterface::NodeType T>
bool NonTerminal<T>::ProcessToken(
    TokenInProcessCollection *token_collection) {
  throw "NotImplementedError.";
}

}  // namespace clidoc
