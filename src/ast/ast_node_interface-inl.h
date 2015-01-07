
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

inline Token::Token(TerminalType type)
    : has_value_(false), type_(type) { /* empty */ }

inline Token::Token(TerminalType type, const std::string &value)
    : has_value_(true), type_(type), value_(value) { /* empty */ }

// support std::map.
inline bool Token::operator<(const Token &other) const {
  return value_ < other.value_;
}

// support equality test.
inline bool Token::operator==(const Token &other) const {
  return has_value_ == other.has_value()
         && type_ == other.type()
         && value_ == other.value();
}

inline bool Token::operator!=(const Token &other) const {
  return !(*this== other);
}

inline bool Token::IsEmpty() const {
  // is TerminalType::OTHER or has empty value.
  return type_ == TerminalType::OTHER
         || (has_value_ && value_.empty());
}

// accessors.
inline bool Token::has_value() const {
  return has_value_;
}

inline TerminalType Token::type() const {
  return type_;
}

inline std::string Token::value() const {
  return value_;
}

// mutator.
inline void Token::set_value(const std::string &value) {
  value_ = value;
}

}  // namespace clidoc
