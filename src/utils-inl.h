
namespace clidoc {

// This member function must be marked inline, otherwise a linkage error would
// be raised.
inline std::string NodeInterface::GetIndent(const int &indent) const {
  std::string indent_element = "  ";
  std::ostringstream strm;
  for (int repeat_times = 0; repeat_times < indent; ++repeat_times) {
    strm << indent_element;
  }
  return strm.str();
}

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

template <TerminalType T>
std::string Terminal<T>::ToString(const int &indent) {
  std::ostringstream strm;
  strm << GetIndent(indent) << GetInfo() << std::endl;
  return strm.str();
}

template <TerminalType T>
std::string Terminal<T>::ToString() {
  return GetInfo();
}

template <NonTerminalType T>
std::string NonTerminal<T>::GetInfo() {
  return kNonTermianlClassName.at(T);
}

template <NonTerminalType T>
std::string NonTerminal<T>::ToString(const int &indent) {
  std::ostringstream strm;
  auto prefix = GetIndent(indent);
  strm << prefix << GetInfo() << "(" << std::endl;
  for (auto ptr : children_) {
    strm << ptr->ToString(indent + 1);
  }
  strm << prefix << ")" << std::endl;
  return strm.str();
}

template <NonTerminalType T>
std::string NonTerminal<T>::ToString() {
  std::ostringstream strm;
  strm << GetInfo() << "(";
  int index = 0;
  for (auto ptr : children_) {
    strm << ptr->ToString();
    if (index != children_.size() - 1) {
      strm << ", ";
      ++index;
    }
  }
  strm << ")";
  return strm.str();
}

}  // namespace clidoc
