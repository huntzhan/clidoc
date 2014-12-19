
#include <map>
#include <string>
#include "generated_parser.h"
#include "utils.h"
#include "parser_proxy.h"

using std::string;

namespace clidoc {

void RepresentativeOptionProperty::set_option_argument(
    const Token &option_argument) {
  if (has_option_argument_) { throw "NotImplementedError."; }
  has_option_argument_ = true;
  option_argument_ = option_argument;
}

void RepresentativeOptionProperty::set_default_value(
    const std::string &default_value) {
  if (has_default_value_) { throw "NotImplementedError."; }
  has_default_value_ = true;
  default_value_ = default_value;
}

Token OptionBindingRecorder::GetRepresentativeOption(
    OptionBindingContainer::SharedPtr container_ptr) {
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
    auto pos_iter = option_to_representative_option_.find(option);
    if (pos_iter != option_to_representative_option_.end()) {
      // binding already exists.
      representative_option_ptr = &pos_iter->second;
      break;
    }
  }
  if (representative_option_ptr == nullptr) {
    throw "NotImplementedError.";
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
        throw "NotImplementedError.";
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

void UpdateRepresentativeOptionProperty(
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
      throw "NotImplementedError.";
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
      throw "NotImplementedError.";
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
  auto pos_iter = representative_option_to_property_.find(
      representative_option);
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
    throw "NotImplementedError.";
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

}  // namespace clidoc
