#ifndef SRC_PARSER_PROXY_H_
#define SRC_PARSER_PROXY_H_

#include <map>
#include <string>
#include "generated_parser.h"
#include "utils.h"

namespace clidoc {

struct RepresentativeOptionProperty {
  // mutators.
  void set_option_argument(const Token &option_argument);
  void set_default_value(const std::string &default_value);

  bool has_option_argument_ = false;
  bool has_default_value_ = false;
  Token option_argument_;
  std::string default_value_;
};

// Record different kinds of optino binding during parsing.
class OptionBindingRecorder {
 public:
  void RecordBinding(
      OptionBindingContainer::SharedPtr container_ptr,
      DefaultValue::SharedPtr default_value_ptr);
  void RecordBinding(const Token &option, const Token &option_argument);

  std::map<Token, Token> option_to_representative_option_;
  std::map<Token, RepresentativeOptionProperty>
      representative_option_to_property_;
  std::map<Token, Token> option_to_option_argument_cache_;

 private:
  Token GetRepresentativeOption(
      OptionBindingContainer::SharedPtr container_ptr);
  Token GetBoundOptionArgument(
      const Token &representative_option,
      OptionBindingContainer::SharedPtr container_ptr);
  void CreateRepresentativeOptionProperty(
      const Token &representative_option,
      const Token &bound_option_argument,
      const Token &default_value);
  void UpdateRepresentativeOptionProperty(
      const Token &representative_option,
      const Token &bound_option_argument,
      const Token &default_value,
      RepresentativeOptionProperty *property_ptr);
};

class ParserProxy {
 public:
};

}  // namespace clidoc
#endif  // SRC_PARSER_PROXY_H_
