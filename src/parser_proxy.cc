
#include <fstream>
#include <iterator>
#include <map>
#include <regex>
#include <sstream>
#include <string>

#include "generated_scanner.h"
#include "generated_parser.h"
#include "node_interface.h"
#include "parser_proxy.h"
#include "utils.h"

using std::string;
using std::regex;
using std::smatch;
using std::regex_replace;
using std::regex_search;
using std::back_inserter;
using std::ofstream;
using std::istringstream;

namespace clidoc {

void RepresentativeOptionProperty::set_option_argument(
    const Token &option_argument) {
  if (has_option_argument_) { throw "NotImplementedError."; }
  has_option_argument_ = true;
  option_argument_ = option_argument;
}

void RepresentativeOptionProperty::set_default_value(
    const string &default_value) {
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

string DocPreprocessor::RemoveComment(const string &text) {
  regex pattern("#.*");
  return regex_replace(text, pattern, "");
}

string DocPreprocessor::RemoveEmptyLine(const string &text) {
  regex pattern("(\n[ \t]*)+");
  return regex_replace(text, pattern, "\n");
}

bool DocPreprocessor::ExtractSection(
    const string &section_name,
    const string &text,
    string *output) {
  regex target_section_pattern(
      "(" + section_name + ")[ \t]*:",
      std::regex_constants::icase);
  regex next_section_pattern(
      "(.+?)[ \t]*:",
      std::regex_constants::icase);
  smatch match_result;
  auto pos_iter = text.cbegin();
  auto text_end_iter = text.cend();

  bool is_success = false;
  // search section_name.
  is_success = regex_search(
      pos_iter, text_end_iter,
      match_result,
      target_section_pattern);
  if (!is_success) {
    // can't find the section.
    return false;
  }

  auto section_begin_iter = match_result[0].first;
  pos_iter = match_result.suffix().first;
  
  // seach enf of section.
  is_success = regex_search(
      pos_iter, text_end_iter,
      match_result,
      next_section_pattern);
  auto section_end_iter = is_success ? match_result[0].first : text_end_iter;

  // remove blank in section name.
  string extract_text(section_begin_iter, section_end_iter);
  *output = regex_replace(
      extract_text,
      next_section_pattern,
      "$1:");
  return true;
}

string DocPreprocessor::ExtractAndProcessUsageSection(const string &text) {
  string usage_section;
  if (!ExtractSection("Usage", text, &usage_section)) {
    // can't find usage section, which must exist.
    throw "NotImplementedError.";
  }
  regex utility_name_pattern(
      "usage:\\s*(\\S+)",
      std::regex_constants::icase);
  smatch match_result;
  if (!regex_search(usage_section, match_result, utility_name_pattern)) {
    throw "NotImplementedError.";
  }
  string utility_name = match_result.str(1);
  return regex_replace(
      usage_section,
      regex("(?:" + utility_name + ")"),
      "*UTILITY_DELIMITER*");
}

string DocPreprocessor::ExtractAndProcessOptionsSection(
    const string &text) {
  string options_section;
  if (!ExtractSection("Options", text, &options_section)) {
    // Since options section is optional, if the program cannot find such
    // section, the program will return an empty string.
    return "";
  }
  // insertion point:
  // Options: \n[NOT here]
  //   -x, -xxx # brbrbr. \n[HERE!]
  //   ...
  regex options_section_name_pattern(
      "options:\\s*",
      std::regex_constants::icase);
  smatch match_result;
  // have no chance to fail.
  regex_search(options_section, match_result, options_section_name_pattern);
  if (match_result.suffix().first == options_section.end()) {
    // case happen if nothing is following the name of options section
    // ("Options:").
    throw "NotImplementedError.";
  }
  string result = match_result.str();
  auto pos_iter = match_result.suffix().first;
  regex_replace(
      back_inserter(result),
      pos_iter, options_section.cend(),
      regex("\n"),
      "\n*DESC_DELIMITER*");
  return result;
}

string ParserProxy::PreprocessRawDoc(const string &raw_doc) {
  string internal_text;
  // remove string.
  internal_text = DocPreprocessor::RemoveComment(raw_doc);
  internal_text = DocPreprocessor::RemoveEmptyLine(internal_text);
  // extract and preprocess usage section.
  string usage_section =
      DocPreprocessor::ExtractAndProcessUsageSection(internal_text);
  // extract and preprocess options section.
  string options_section =
      DocPreprocessor::ExtractAndProcessOptionsSection(internal_text);
  return usage_section + options_section;
}

void ParserProxy::ParseByBison(
    const string &preprocessed_doc,
    Doc::SharedPtr *doc_ptr,
    OptionBindingRecorder *option_binding_recorder_ptr) {
  // setup scanner.
  ofstream null_ostream("/dev/null");
  istringstream input_stream(preprocessed_doc);
  FlexGeneratedScanner lexer(&input_stream, &null_ostream);
  // init parser.
  BisonGeneratedParser bison_parser(
      &lexer, doc_ptr, option_binding_recorder_ptr);
  bison_parser.parse();
}

}  // namespace clidoc
