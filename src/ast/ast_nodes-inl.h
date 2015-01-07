
namespace clidoc {

template <TerminalType T>
inline Terminal<T>::Terminal(const Token &token) : token_(token) {
  if (token.type() != T) { throw std::logic_error("Terminal"); }
}

template <TerminalType T>
inline Terminal<T>::Terminal(const std::string &value)
    : token_(T, value) { /* empty */ }

template <TerminalType T>
std::string Terminal<T>::GetID() {
  std::string token_value = token_.has_value() ? token_.value() : "NoValue";
  return kTermianlClassName.at(T) + "[" + token_value + "]";
}

template <TerminalType T>
std::string Terminal<T>::ToString() {
  return GetID();
}

template <TerminalType T>
std::string Terminal<T>::ToString(const int &indent) {
  std::ostringstream strm;
  strm << GetIndent(indent) << GetID() << std::endl;
  return strm.str();
}

template <TerminalType T>
void Terminal<T>::Accept(NodeVistorInterface *visitor_ptr) {
  visitor_ptr->ProcessNode(this->shared_from_this());
}

template <NonTerminalType T>
std::string NonTerminal<T>::GetID() {
  return kNonTermianlClassName.at(T);
}

template <NonTerminalType T>
std::string NonTerminal<T>::ToString() {
  std::ostringstream strm;
  strm << GetID() << "(";
  std::size_t index = 0;
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

template <NonTerminalType T>
std::string NonTerminal<T>::ToString(const int &indent) {
  std::ostringstream strm;
  auto prefix = GetIndent(indent);
  strm << prefix << GetID() << "(" << std::endl;
  for (auto ptr : children_) {
    strm << ptr->ToString(indent + 1);
  }
  strm << prefix << ")" << std::endl;
  return strm.str();
}

template <NonTerminalType T>
void NonTerminal<T>::Accept(NodeVistorInterface *visitor_ptr) {
  visitor_ptr->ProcessNode(this->shared_from_this());
}

template <NonTerminalType T>
void NonTerminal<T>::AddChild(SharedPtrNode node_ptr) {
  children_.push_back(node_ptr);
  // make connection.
  node_ptr->node_connection.ConnectParent(this);
}

}  // namespace clidoc
