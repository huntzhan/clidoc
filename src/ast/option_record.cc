
#include <stdexcept>
#include <string>
#include <set>

#include "ast/ast_node_interface.h"
#include "ast/option_record.h"

using std::string;
using std::set;
using std::logic_error;

namespace clidoc {

OptionBinding::OptionBinding(const Token &token_option)
    : token_option_(token_option) { /* empty */ }

OptionBinding::OptionBinding(const Token &token_option,
                             const Token &token_optin_argument)
    : token_option_(token_option),
      token_option_argument_(token_optin_argument) { /* empty */ }

void OptionBindingContainer::AddChild(OptionBinding::SharedPtr node_ptr) {
    children_.push_back(node_ptr);
}

DefaultValue::DefaultValue(const Token &default_value)
    : default_value_(default_value) {
  // the default value is surrounded by double-quote marks, extract it.
  string raw_text = default_value_.value();
  string extracted_text(raw_text.begin() + 1, raw_text.end() - 1);
  if (!default_value_.IsEmpty()) {
    default_value_.set_value(extracted_text);
  }
}

bool RepresentativeOptionProperty::IsEmpty() const {
  return option_argument_.IsEmpty();
}

void RepresentativeOptionProperty::set_option_argument(
    const Token &option_argument) {
  if (has_option_argument_) {
    throw logic_error("set_option_argument error");
  }
  has_option_argument_ = true;
  option_argument_ = option_argument;
}

void RepresentativeOptionProperty::set_default_value(
    const string &default_value) {
  if (has_default_value_) {
    throw logic_error("set_default_value");
  }
  has_default_value_ = true;
  default_value_ = default_value;
}

Token OptionBindingRecorder::GetRepresentativeOption(
    OptionBindingContainer::SharedPtr container_ptr) {
  if (container_ptr->children_.empty()) {
    throw logic_error("GetRepresentativeOption");
  }
  // Get `representative_option`. If no `representative_option` is found in
  // `option_to_representative_option_`, then the first GNU_OPTION would be
  // selected as `representative_option`.
  const Token *representative_option_ptr = nullptr;
  for (auto binding_ptr : container_ptr->children_) {
    const Token &option = binding_ptr->token_option_;
    // catch first GNU_OPTION.
    if (option.type() == TerminalType::GNU_OPTION
        && representative_option_ptr == nullptr) {
      representative_option_ptr = &option;
    }
    // search in `option_to_representative_option_`.
    if (IsRecorded(option)) {
      // binding already exists.
      representative_option_ptr =
          &option_to_representative_option_[option];
      break;
    }
  }
  if (representative_option_ptr == nullptr) {
    // there is no GNU_OPTION, and no recorded bindings.
    representative_option_ptr =
        &container_ptr->children_.front()  // first OptionBinding.
                      ->token_option_;     // token.
  }
  return Token(*representative_option_ptr);
}

Token OptionBindingRecorder::GetBoundOptionArgument(
    const Token &representative_option,
    OptionBindingContainer::SharedPtr container_ptr) {
  // fill `option_to_representative_option_` and extract `option_argument`.
  const Token *option_argument_ptr = nullptr;
  for (auto binding_ptr : container_ptr->children_) {
    const Token &option = binding_ptr->token_option_;
    const Token &option_argument = binding_ptr->token_option_argument_;
    // create binding.
    option_to_representative_option_[option] = representative_option;
    // get `option_argument`.
    if (!option_argument.IsEmpty()) {
      // bound argument exists.
      if (option_argument_ptr == nullptr) {
        option_argument_ptr = &option_argument;
      } else if (*option_argument_ptr != option_argument) {
        throw logic_error("GetBoundOptionArgument");
      }
    }
  }
  if (option_argument_ptr == nullptr) {
    return Token();
  } else {
    return Token(*option_argument_ptr);
  }
}

void OptionBindingRecorder::CreateRepresentativeOptionProperty(
    const Token &representative_option,
    const Token &bound_option_argument,
    const Token &default_value) {
  // `representative_option` not exist, which means property not exists.
  RepresentativeOptionProperty property;
  if (!bound_option_argument.IsEmpty()) {
    // has bound option argument.
    property.set_option_argument(bound_option_argument);
  }
  if (!default_value.IsEmpty()) {
    // has default value.
    property.set_default_value(default_value.value());
  }
  // keep mapping.
  representative_option_to_property_[representative_option] = property;
}

void OptionBindingRecorder::UpdateRepresentativeOptionProperty(
    const Token &representative_option,
    const Token &bound_option_argument,
    const Token &default_value,
    RepresentativeOptionProperty *property_ptr) {
  // deal with option_argument.
  if (property_ptr->has_option_argument_) {
    // check equality.
    if (!bound_option_argument.IsEmpty()
        && property_ptr->option_argument_ != bound_option_argument) {
      // option argument not match!
      throw logic_error("UpdateRepresentativeOptionProperty_1");
    }
  } else if (!bound_option_argument.IsEmpty()) {
    // no option_argument has been set, just set it.
    property_ptr->set_option_argument(bound_option_argument);
  }

  // deal with default value.
  if (property_ptr->has_default_value_) {
    // check equality.
    if (!default_value.IsEmpty()
        && property_ptr->default_value_ != default_value.value()) {
      throw logic_error("UpdateRepresentativeOptionProperty_2");
    }
  } else if (!default_value.IsEmpty()) {
    // similar with option argument.
    property_ptr->set_default_value(default_value.value());
  }
}

void OptionBindingRecorder::RecordBinding(
    OptionBindingContainer::SharedPtr container_ptr,
    DefaultValue::SharedPtr default_value_ptr) {

  Token representative_option = GetRepresentativeOption(container_ptr);
  Token bound_option_argument = GetBoundOptionArgument(
      representative_option, container_ptr);

  // fill `representative_option_to_property_`.
  auto pos_iter =
      representative_option_to_property_.find(representative_option);
  if (pos_iter == representative_option_to_property_.end()) {
    CreateRepresentativeOptionProperty(
        representative_option,
        bound_option_argument,
        default_value_ptr->default_value_);
  } else {
    // property exists.
    UpdateRepresentativeOptionProperty(
        representative_option,
        bound_option_argument,
        default_value_ptr->default_value_,
        &pos_iter->second);
  }
}

void OptionBindingRecorder::RecordBinding(
    const Token &option, const Token &option_argument) {
  auto pos_iter = option_to_option_argument_cache_.find(option);
  if (pos_iter == option_to_option_argument_cache_.end()) {
    option_to_option_argument_cache_[option] = option_argument;
  } else if (option_argument != pos_iter->second) {
    throw logic_error("RecordBinding");
  }
}

void OptionBindingRecorder::ProcessCachedBindings() {
  for (const auto &binding : option_to_option_argument_cache_) {
    const Token &option = binding.first;
    const Token &option_argument = binding.second;
    // get representative_option.
    if (option_to_representative_option_.find(option)
        == option_to_representative_option_.end()) {
      // no corresponding representative_option.
      // just bind to itself.
      option_to_representative_option_[option] = option;
    }
    const auto &representative_option =
        option_to_representative_option_[option];
    // handle option_argument.
    auto pos_iter =
        representative_option_to_property_.find(representative_option);
    if (pos_iter == representative_option_to_property_.end()) {
      CreateRepresentativeOptionProperty(
          representative_option,
          option_argument,
          Token());
    } else {
      UpdateRepresentativeOptionProperty(
          representative_option,
          option_argument,
          Token(),
          &pos_iter->second);
    }
  }
}

bool OptionBindingRecorder::IsRecorded(const Token &option) const {
  return option_to_representative_option_.find(option)
         != option_to_representative_option_.end();
}

bool OptionBindingRecorder::HasArgument(const Token &option) const {
  if (!IsRecorded(option)) {
    return false;
  }
  const auto &rep_option = option_to_representative_option_.at(option);
  return !representative_option_to_property_.at(rep_option).IsEmpty();
}

set<Token> OptionBindingRecorder::GetBoundArguments() const {
  set<Token> bound_arguments;
  for (const auto &map_pair : representative_option_to_property_) {
    const auto &property = map_pair.second;
    if (property.has_option_argument_) {
      bound_arguments.insert(property.option_argument_);
    }
  }
  return bound_arguments;
}

}  // namespace clidoc
