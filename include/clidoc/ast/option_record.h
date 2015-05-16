#ifndef INCLUDE_CLIDOC_AST_OPTION_RECORD_H_
#define INCLUDE_CLIDOC_AST_OPTION_RECORD_H_

#include <map>
#include <vector>
#include <set>
#include <string>

#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/ast/smart_ptr_interface.h"

namespace clidoc {

// For capturing option bindings.
class OptionBinding : public SmartPtrInterface<OptionBinding> {
 public:
  // binding without argument, for synonym options, i.e. "-h, --help".
  explicit OptionBinding(const Token &token_option);
  // binding option and option_argument.
  OptionBinding(const Token &option, const Token &optin_argument);

  const Token option_;
  const Token option_argument_ = Token();
};

class OptionBindingContainer
    : public SmartPtrInterface<OptionBindingContainer> {
 public:
  void AddChild(OptionBinding::SharedPtr node);
  std::vector<OptionBinding::SharedPtr> children_;
};

// For capturing default value of option(s) argument.
class DefaultValue : public SmartPtrInterface<DefaultValue> {
 public:
  DefaultValue() = default;
  explicit DefaultValue(const std::string &default_value_text);

  Token default_value_;
};

class RepresentativeOptionProperty {
 public:
  void Mutate(const Token &option_argument, const Token &default_value);
  bool IsEmpty() const;
  bool HasDefaultValue() const;

  Token option_argument_;
  std::string default_value_;

 private:
  // invoked by mutators.
  template <typename Target, typename Flag, typename ValueType>
  friend void SetDataMember(
      RepresentativeOptionProperty *ptr,
      Target target, Flag flag, const ValueType &value);
  // mutators.
  void set_option_argument(const Token &option_argument);
  void set_default_value(const std::string &default_value);
  bool has_option_argument_ = false;
  bool has_default_value_ = false;
};

template <typename Target, typename Flag, typename ValueType>
void SetDataMember(
    RepresentativeOptionProperty *ptr,
    Target target, Flag flag, const ValueType &value);

// Record different kinds of optino binding during parsing.
class OptionBindingRecorder {
 public:
  void RecordBinding(
      OptionBindingContainer::SharedPtr node_container,
      DefaultValue::SharedPtr default_value_node);
  // `RecordBinding(const Token &option, const Token &option_argument)` do
  // nothing but record the token pairs in cache. `ProcessCachedBindings` must
  // be called for actual processing.
  void RecordBinding(const Token &option, const Token &option_argument);
  void ProcessCachedBindings();
  // `RecordBinding(const Token &option)` record a single unbound option.
  void RecordSingleOption(const Token &option);

  // check if option is recorded.
  bool OptionIsRecorded(const Token &option) const;
  // check if option is bound with non-empty argument.
  bool OptionIsBound(const Token &option) const;

  std::set<Token> GetRepresentativeOptions() const;
  std::set<Token> GetBoundArguments() const;

  RepresentativeOptionProperty
      &GetBoundProperty(const Token &option);
  const RepresentativeOptionProperty
      &GetBoundProperty(const Token &option) const;

  std::map<Token, Token> option_to_representative_option_;
  std::map<Token, RepresentativeOptionProperty>
      representative_option_to_property_;

 private:
  // check if bound property exist.
  bool PropertyIsExist(const Token &option) const;

  // select a representative option from `node_container`(if not recorded) and
  // `option_to_option_argument_cache_`(if recorded).
  Token GetRepresentativeOptionsFromContainer(
      OptionBindingContainer::SharedPtr node_container);
  // set representative option.
  void SetRepresentativeOptionFromContainer(
      const Token &representative_option,
      OptionBindingContainer::SharedPtr node_container);
  // select bound argument from `node_container`. If there's no bound argument,
  // return an empty token.
  Token GetBoundOptionArgumentFromContainer(
      OptionBindingContainer::SharedPtr node_container);

  // created property if:
  //   1. property not exist(premise).
  //   2. `bound_option_argument` or `default_value` not empty.
  void CreateRepresentativeOptionProperty(
      const Token &representative_option,
      const Token &bound_option_argument,
      const Token &default_value);
  // property exist, meaning that bound argument is not empty.
  void UpdateRepresentativeOptionProperty(
      const Token &bound_option_argument,
      const Token &default_value,
      RepresentativeOptionProperty *property_ptr);

  // store option -> option_argument binding when parsing usage section.
  std::map<Token, Token> option_to_option_argument_cache_;
};

// TODO(huntzhan): refactor classes related to parsing.
class UnboundArgumentDefaultValueRecorder {
 public:
  void RecordUnboundArgumentDefaultValue(
      const Token &unbound_argument,
      const Token &default_value);
  std::map<Token, std::string> unbound_argument_to_default_value_;
};

}  // namespace clidoc

#endif  // INCLUDE_CLIDOC_AST_OPTION_RECORD_H_
