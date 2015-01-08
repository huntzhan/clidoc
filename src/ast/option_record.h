#ifndef SRC_AST_OPTION_RECORD_H_
#define SRC_AST_OPTION_RECORD_H_

#include <map>
#include <vector>
#include <string>

#include "ast/ast_node_interface.h"
#include "shared/smart_ptr_interface.h"

namespace clidoc {

// For capturing option bindings.
class OptionBinding : public SmartPtrInterface<OptionBinding> {
 public:
  // binding without argument, for synonym options, i.e. "-h, --help".
  explicit OptionBinding(const Token &token_option);
  // binding option and option_argument.
  OptionBinding(const Token &token_option, const Token &token_optin_argument);

  const Token token_option_;
  const Token token_option_argument_ = Token();
};

class OptionBindingContainer
    : public SmartPtrInterface<OptionBindingContainer> {
 public:
  void AddChild(OptionBinding::SharedPtr node_ptr);
  std::vector<OptionBinding::SharedPtr> children_;
};

// For capturing default value of option(s) argument.
class DefaultValue : public SmartPtrInterface<DefaultValue> {
 public:
  DefaultValue() = default;
  explicit DefaultValue(const Token &default_value);

  Token default_value_;
};

struct RepresentativeOptionProperty {
  bool IsEmpty() const;
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
  // `RecordBinding(const Token &option, const Token &option_argument)` do
  // nothing but record the token pairs in cache. `ProcessCachedBindings` must
  // be called for actual processing.
  void RecordBinding(const Token &option, const Token &option_argument);
  void ProcessCachedBindings();

  std::map<Token, Token> option_to_representative_option_;
  std::map<Token, RepresentativeOptionProperty>
      representative_option_to_property_;

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

  // store option -> option_argument binding when parsing usage section.
  std::map<Token, Token> option_to_option_argument_cache_;
};

}  // namespace clidoc

#endif  // SRC_AST_OPTION_RECORD_H_
