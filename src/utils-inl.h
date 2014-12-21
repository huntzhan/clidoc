
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

template <TerminalType T>
std::string Terminal<T>::GetInfo() {
  std::string token_value = token_.has_value() ? token_.value() : "NoValue";
  return kTermianlClassName.at(T) + "[" + token_value + "]";
}

template <NonTerminalType T>
std::string NonTerminal<T>::GetInfo() {
  return kNonTermianlClassName.at(T);
}

template <TerminalType T>
std::string Terminal<T>::ToString() {
  return GetInfo();
}

template <NonTerminalType T>
std::string NonTerminal<T>::ToString() {
  std::string info = GetInfo() + "(";
  int index = 0;
  for (auto ptr : children_) {
    info += ptr->ToString();
    if (index != children_.size() - 1) {
      info += ", ";
      ++index;
    }
  }
  info += ")";
  return info;
}

}  // namespace clidoc
