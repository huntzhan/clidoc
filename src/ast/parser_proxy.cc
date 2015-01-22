
#include <stdexcept>
#include <cstddef>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <set>

#include "ast/generated_scanner.h"
#include "ast/generated_parser.h"
#include "ast/parser_proxy.h"
#include "ast/ast_nodes.h"
#include "ast/ast_node_interface.h"
#include "ast/option_record.h"

using std::string;
using std::regex;
using std::smatch;
using std::regex_replace;
using std::regex_search;
using std::regex_match;
using std::back_inserter;
using std::istringstream;
using std::ostringstream;
using std::vector;
using std::size_t;
using std::to_string;
using std::logic_error;
using std::set;

namespace clidoc {

bool StringUtils::ExtractSection(
    const string &section_name,
    const string &text,
    string *output) {
  regex target_section_pattern(
      "(" + section_name + ")[ \t]*:",
      // case-insensitive.
      std::regex_constants::icase);
  regex next_section_pattern(
      "(\\w+?)[ \t]*:",
      std::regex_constants::icase);
  smatch match_result;
  auto pos_iter = text.cbegin();
  auto text_end_iter = text.cend();

  bool found = false;
  // search the begin of section.
  found = regex_search(
      pos_iter, text_end_iter,
      match_result,
      target_section_pattern);
  if (!found) {
    // can't find the section.
    return false;
  }
  auto section_begin_iter = match_result[0].first;
  pos_iter = match_result.suffix().first;

  // seach the end of section.
  found = regex_search(
      pos_iter, text_end_iter,
      match_result,
      next_section_pattern);

  auto section_end_iter = text_end_iter;
  if (found) {
    // ignroe `[default: "xxx"]`.
    bool found = regex_match(
        match_result.str(1),
        regex("default", std::regex_constants::icase));
    if (!found) {
      section_end_iter = match_result[0].first;
    }
  }

  // remove blank in section name.
  string extract_text(section_begin_iter, section_end_iter);
  *output = regex_replace(
      extract_text,
      target_section_pattern,
      "$1:");
  return true;
}

void StringUtils::ReplaceAll(
    const string &old_substring,
    const string &new_substring,
    string *text_ptr) {
  if (old_substring.empty()) { return; }
  size_t pos = 0;
  while ((pos = text_ptr->find(old_substring, pos)) != string::npos) {
    // find a substring to be replaced.
    text_ptr->replace(pos, old_substring.size(), new_substring);
    pos += new_substring.size();
  }
}

void StringUtils::ReplaceElementWithMark(
    const regex &pattern,
    std::string *text_ptr,
    std::vector<std::string> *elements_ptr) {
  smatch match_result;
  while (regex_search(*text_ptr, match_result, pattern)) {
    auto element = match_result.str(1);
    // ` __id__ `. Notice that `__id__` is surrounded by spaces.
    string content = " __" + to_string(elements_ptr->size()) + "__ ";
    StringUtils::ReplaceAll(element, content, text_ptr);
    elements_ptr->push_back(element);
  }
}

void StringUtils::InsertSpace(
    const std::vector<std::string> &regex_strings,
    const std::vector<std::string> &keywords,
    std::string *text_ptr) {
  auto elements = ReplaceSpeicalElement(regex_strings, text_ptr);
  for (const auto &keyword : keywords) {
    StringUtils::ReplaceAll(keyword, " " + keyword + " ", text_ptr);
  }
  // collapse whitespaces.
  *text_ptr = regex_replace(*text_ptr, regex("\\s+"), " ");
  if (text_ptr->back() == ' ') {
    text_ptr->pop_back();
  }
  RestoreSpeicalElement(elements, text_ptr);
}

vector<string> StringUtils::ReplaceSpeicalElement(
    const std::vector<string> &regex_strings,
    std::string *text_ptr) {
  vector<string> elements;
  for (const string &regex_string : regex_strings) {
    StringUtils::ReplaceElementWithMark(
        regex(regex_string),
        text_ptr,
        &elements);
  }
  return elements;
}

void StringUtils::RestoreSpeicalElement(
    const std::vector<std::string> &elements,
    std::string *text_ptr) {
  int index = 0;
  for (const string &element : elements) {
    string content = "__" + to_string(index) + "__";
    StringUtils::ReplaceAll(content, element, text_ptr);
    ++index;
  }
}

void DocPreprocessor::RemoveComment() {
  regex pattern("#.*");  // must NOT remove the tailing NEWLINE character.
  text_ = regex_replace(text_, pattern, "");
}

void DocPreprocessor::RemoveEmptyLine() {
  regex pattern("(\n[ \t]*)+");
  text_ = regex_replace(text_, pattern, "\n");
}

void DocPreprocessor::ReplaceUtilityName() {
  regex utility_name_pattern(
      "usage:\\s*(\\S+)",
      std::regex_constants::icase);
  smatch match_result;
  bool found = regex_search(
      usage_section_,
      match_result,
      utility_name_pattern);
  if (!found) {
    throw logic_error("ReplaceUtilityName");
  }
  string utility_name = match_result.str(1);
  StringUtils::ReplaceAll(
      utility_name, "*UTILITY_DELIMITER*", &usage_section_);
}

void DocPreprocessor::InsertDesDelimiter() {
  // insertion point:
  // Options: \n[NOT here]
  //   -x, -xxx # brbrbr. \n[HERE!]
  //   ...
  regex options_section_name_pattern(
      "options:\\s*",
      std::regex_constants::icase);
  smatch match_result;
  // have no chance to fail.
  regex_search(options_section_, match_result, options_section_name_pattern);
  if (match_result.suffix().first == options_section_.end()) {
    // case happen if nothing is following the name of options section
    // ("Options:").
    throw logic_error("InsertDesDelimiter");
  }
  string processed_options_section = match_result.str();
  auto pos_iter = match_result.suffix().first;
  regex_replace(
      back_inserter(processed_options_section),
      pos_iter, options_section_.cend(),
      regex("\n"),
      "\n*DESC_DELIMITER*");
  options_section_ = processed_options_section;
}

void DocPreprocessor::DisambiguateByInsertSpace() {
  const vector<string> keywords = {
    "(",
    ")",
    "[",
    "]",
    "|",
    "...",
    "=",
    "options",
    "*UTILITY_DELIMITER*",
    "*DESC_DELIMITER*" ,
  };

  // deal with `usage_section_`.
  usage_section_ += "*UTILITY_DELIMITER*";
  StringUtils::InsertSpace(
      {"(-- (.|\n)*?)(\\s*)(\\*UTILITY_DELIMITER\\*)", "(<.*?>)"},
      keywords,
      &usage_section_);
  // remove previous added dilimiter.
  usage_section_ = regex_replace(
      usage_section_,
      regex("\\*UTILITY_DELIMITER\\*$"),
      "");

  // deal with `options_section_`.
  StringUtils::InsertSpace(
      {"(<.*?>)", "(\".*?\")"},
      keywords,
      &options_section_);

  // update `text_`.
  text_ = usage_section_ + options_section_;
}

void DocPreprocessor::ExtractAndProcessUsageSection() {
  if (StringUtils::ExtractSection("Usage", text_, &usage_section_)) {
    ReplaceUtilityName();
  } else {
    // can't find usage section, which must exist.
    throw logic_error("ExtractAndProcessUsageSection");
  }
}

void DocPreprocessor::ExtractAndProcessOptionsSection() {
  if (StringUtils::ExtractSection("Options", text_, &options_section_)) {
    InsertDesDelimiter();
  } else {
    // Since options section is optional, if the program cannot find such
    // section, the program will return an empty string.
    options_section_ = "";
  }
}

string DocPreprocessor::PreprocessRawDocForParsing(const string &raw_doc) {
  text_ = raw_doc;
  // remove string.
  RemoveComment();
  RemoveEmptyLine();
  // extract and preprocess usage section.
  ExtractAndProcessUsageSection();
  // extract and preprocess options section.
  ExtractAndProcessOptionsSection();
  // disambiguate.
  DisambiguateByInsertSpace();
  return text_;
}

string DocPreprocessor::PreprocessRawDocForCodeGen(const string &raw_doc) {
  regex pattern("#[ \t]*");  // Replace # and following space or tab.
  return regex_replace(raw_doc, pattern, "");
}

string ParserProxy::PreprocessRawDoc(const string &raw_doc) {
  DocPreprocessor preprocessor;
  return preprocessor.PreprocessRawDocForParsing(raw_doc);
}

void ParserProxy::ParseByBison(
    const string &preprocessed_doc,
    Doc::SharedPtr *doc_node_ptr,
    OptionBindingRecorder *recorder_ptr) {
  // setup scanner.
  ostringstream null_ostream;
  istringstream input_stream(preprocessed_doc);
  FlexGeneratedScanner lexer(&input_stream, &null_ostream);
  // init parser.
  BisonGeneratedParser bison_parser(
      &lexer, doc_node_ptr, recorder_ptr);
  bison_parser.parse();

  // postprocess.
  recorder_ptr->ProcessCachedBindings();
  // free smart pointer cached.
  SPIStaticDataMember::FreeCache();
}

void ParserProxy::Parse(
    const std::string &doc,
    Doc::SharedPtr *doc_node_ptr,
    OptionBindingRecorder *recorder_ptr) {
  auto preprocessed_doc = PreprocessRawDoc(doc);
  ParseByBison(preprocessed_doc, doc_node_ptr, recorder_ptr);
}

}  // namespace clidoc
