
#include <iostream>
#include <cstdlib>

#include <stdexcept>
#include <string>
#include <set>

#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/ast/option_record.h"

using std::string;
using std::set;
using std::logic_error;

namespace clidoc {

void BoundArgumentMismatchLogging(const string &left, const string &right) {
  std::cout << "bound argument mismatch: "
            << left << " != " << right
            << std::endl;
  std::exit(1);
}

void BoundArgumentMismatchLogging(const Token &left, const Token &right) {
  BoundArgumentMismatchLogging(left.value(), right.value());
}

OptionBinding::OptionBinding(const Token &option)
    : option_(option) { /* empty */ }

OptionBinding::OptionBinding(const Token &option,
                             const Token &optin_argument)
    : option_(option),
      option_argument_(optin_argument) { /* empty */ }

void OptionBindingContainer::AddChild(OptionBinding::SharedPtr node) {
    children_.push_back(node);
}

DefaultValue::DefaultValue(const string &default_value_text)
    : default_value_(TerminalType::DEFAULT_VALUE, default_value_text) {
  // the default value is surrounded by double-quote marks, extract it.
  string raw_text = default_value_.value();
  string extracted_text(raw_text.begin() + 1, raw_text.end() - 1);
  if (!default_value_.IsEmpty()) {
    default_value_.set_value(extracted_text);
  }
}

void RepresentativeOptionProperty::Mutate(
    const Token &option_argument, const Token &default_value) {
  if (!option_argument.IsEmpty()) {
    set_option_argument(option_argument);
  }
  // deal with default value.
  if (!default_value.IsEmpty()) {
    set_default_value(default_value.value());
  }
}

bool RepresentativeOptionProperty::IsEmpty() const {
  return option_argument_.IsEmpty();
}

bool RepresentativeOptionProperty::HasDefaultValue() const {
  return has_default_value_;
}

template <typename Target, typename Flag, typename ValueType>
void SetDataMember(
    RepresentativeOptionProperty *ptr,
    Target target, Flag flag, const ValueType &value) {
  if (ptr->*flag) {
    if (ptr->*target != value) {
      BoundArgumentMismatchLogging(ptr->*target, value);
    }
    return;
  }
  ptr->*flag = true;
  ptr->*target = value;
}

void RepresentativeOptionProperty::set_option_argument(
    const Token &option_argument) {
  SetDataMember(
      this,
      &RepresentativeOptionProperty::option_argument_,
      &RepresentativeOptionProperty::has_option_argument_,
      option_argument);
}

void RepresentativeOptionProperty::set_default_value(
    const string &default_value) {
  SetDataMember(
      this,
      &RepresentativeOptionProperty::default_value_,
      &RepresentativeOptionProperty::has_default_value_,
      default_value);
}

Token OptionBindingRecorder::GetRepresentativeOptionsFromContainer(
    OptionBindingContainer::SharedPtr node_container) {
  if (node_container->children_.empty()) {
    throw logic_error("GetRepresentativeOptionsFromContainer");
  }
  // Get `representative_option`. If no `representative_option` is found in
  // `option_to_representative_option_`, then the first GNU_OPTION would be
  // selected as `representative_option`.
  const Token *representative_option_ptr = nullptr;
  for (auto binding_ptr : node_container->children_) {
    const Token &option = binding_ptr->option_;
    // catch first GNU_OPTION.
    if (option.type() == TerminalType::GNU_OPTION
        && representative_option_ptr == nullptr) {
      representative_option_ptr = &option;
    }
    // search in `option_to_representative_option_`.
    if (OptionIsRecorded(option)) {
      // binding already exists.
      representative_option_ptr =
          &option_to_representative_option_[option];
      break;
    }
  }
  if (representative_option_ptr == nullptr) {
    // there is no GNU_OPTION, and no recorded bindings.
    representative_option_ptr =
        &node_container->children_.front()  // first OptionBinding.
                       ->option_;     // token.
  }
  return *representative_option_ptr;
}

void OptionBindingRecorder::SetRepresentativeOptionFromContainer(
    const Token &representative_option,
    OptionBindingContainer::SharedPtr node_container) {
  for (auto binding_ptr : node_container->children_) {
    const Token &option = binding_ptr->option_;
    // create binding.
    option_to_representative_option_[option] = representative_option;
  }
}

Token OptionBindingRecorder::GetBoundOptionArgumentFromContainer(
    OptionBindingContainer::SharedPtr node_container) {
  // fill `option_to_representative_option_` and extract `option_argument`.
  const Token *option_argument_ptr = nullptr;
  for (auto binding_ptr : node_container->children_) {
    const Token &option_argument = binding_ptr->option_argument_;
    if (!option_argument.IsEmpty()) {
      // bound argument exists.
      if (option_argument_ptr == nullptr) {
        option_argument_ptr = &option_argument;
      } else if (*option_argument_ptr != option_argument) {
        BoundArgumentMismatchLogging(*option_argument_ptr, option_argument);
      }
    }
  }
  return option_argument_ptr == nullptr ? Token() : *option_argument_ptr;
}

void OptionBindingRecorder::CreateRepresentativeOptionProperty(
    const Token &representative_option,
    const Token &bound_option_argument,
    const Token &default_value) {
  // `representative_option` not exist, which means property not exists.
  RepresentativeOptionProperty property;
  property.Mutate(bound_option_argument, default_value);
  if (!property.IsEmpty()) {
    // keep mapping.
    representative_option_to_property_[representative_option] = property;
  }
}

void OptionBindingRecorder::UpdateRepresentativeOptionProperty(
    const Token &bound_option_argument,
    const Token &default_value,
    RepresentativeOptionProperty *property_ptr) {
  // deal with option_argument.
  property_ptr->Mutate(bound_option_argument, default_value);
}

void OptionBindingRecorder::RecordBinding(
    OptionBindingContainer::SharedPtr node_container,
    DefaultValue::SharedPtr default_value_node) {
  // deal with `representative_option`.
  Token representative_option = GetRepresentativeOptionsFromContainer(
      node_container);
  SetRepresentativeOptionFromContainer(representative_option, node_container);
  // deal with bound option argument.
  Token bound_option_argument = GetBoundOptionArgumentFromContainer(
      node_container);

  if (!PropertyIsExist(representative_option)) {
    CreateRepresentativeOptionProperty(
        representative_option,
        bound_option_argument,
        default_value_node->default_value_);
  } else {
    // property exists.
    UpdateRepresentativeOptionProperty(
        bound_option_argument,
        default_value_node->default_value_,
        &GetBoundProperty(representative_option));
  }
}

void OptionBindingRecorder::RecordBinding(
    const Token &option, const Token &option_argument) {
  auto pos_iter = option_to_option_argument_cache_.find(option);
  if (pos_iter == option_to_option_argument_cache_.end()) {
    option_to_option_argument_cache_[option] = option_argument;
  } else if (option_argument != pos_iter->second) {
    BoundArgumentMismatchLogging(option_argument, pos_iter->second);
  }
}

void OptionBindingRecorder::ProcessCachedBindings() {
  for (const auto &binding : option_to_option_argument_cache_) {
    const Token &option = binding.first;
    const Token &option_argument = binding.second;
    // get representative_option.
    if (!OptionIsRecorded(option)) {
      // no corresponding representative_option.
      // just bind to itself.
      option_to_representative_option_[option] = option;
    }
    const auto &representative_option =
        option_to_representative_option_[option];
    Token empty_token;
    // handle option_argument.
    if (!PropertyIsExist(option)) {
      CreateRepresentativeOptionProperty(
          representative_option,
          option_argument,
          empty_token);
    } else {
      UpdateRepresentativeOptionProperty(
          option_argument,
          empty_token,
          &GetBoundProperty(representative_option));
    }
  }
  option_to_option_argument_cache_.clear();
}

void OptionBindingRecorder::RecordSingleOption(const Token &option) {
  RecordBinding(option, Token());
  ProcessCachedBindings();
}

bool OptionBindingRecorder::OptionIsRecorded(const Token &option) const {
  return option_to_representative_option_.find(option)
         != option_to_representative_option_.end();
}

bool OptionBindingRecorder::PropertyIsExist(const Token &option) const {
  if (!OptionIsRecorded(option)) {
    // option not recorded.
    return false;
  }
  const auto &rep_option = option_to_representative_option_.at(option);
  if (representative_option_to_property_.find(rep_option)
      == representative_option_to_property_.end()) {
    // option has no bound property.
    return false;
  }
  return true;
}

bool OptionBindingRecorder::OptionIsBound(const Token &option) const {
  if (!PropertyIsExist(option)) {
    return false;
  }
  return !GetBoundProperty(option).option_argument_.IsEmpty();
}

RepresentativeOptionProperty &OptionBindingRecorder::GetBoundProperty(
    const Token &option) {
  const OptionBindingRecorder *const_this = this;
  const auto &property = const_this->GetBoundProperty(option);
  return const_cast<RepresentativeOptionProperty &>(property);
}

const RepresentativeOptionProperty &OptionBindingRecorder::GetBoundProperty(
    const Token &option) const {
  if (!OptionIsRecorded(option) || !PropertyIsExist(option)) {
    throw logic_error("GetBoundProperty");
  }
  const auto &rep_option = option_to_representative_option_.at(option);
  return representative_option_to_property_.at(rep_option);
}

set<Token> OptionBindingRecorder::GetRepresentativeOptions() const {
  set<Token> rep_options;
  for (const auto &map_pair : option_to_representative_option_) {
    rep_options.insert(map_pair.second);
  }
  return rep_options;
}

set<Token> OptionBindingRecorder::GetBoundArguments() const {
  set<Token> bound_arguments;
  for (const auto &map_pair : representative_option_to_property_) {
    const auto &property = map_pair.second;
    if (!property.option_argument_.IsEmpty()) {
      bound_arguments.insert(property.option_argument_);
    }
  }
  return bound_arguments;
}

void UnboundArgumentDefaultValueRecorder::RecordUnboundArgumentDefaultValue(
    const Token &unbound_argument,
    const Token &default_value) {
  unbound_argument_to_default_value_[unbound_argument] = default_value.value();
}

}  // namespace clidoc
