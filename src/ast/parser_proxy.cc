
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
#include "ast/process_logic.h"
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

bool DocPreprocessor::ExtractSection(
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

void DocPreprocessor::ReplaceAll(
    string *text_ptr,
    const string &old_substring,
    const string &new_substring) {
  if (old_substring.empty()) { return; }
  size_t pos = 0;
  while ((pos = text_ptr->find(old_substring, pos)) != string::npos) {
    // find a substring to be replaced.
    text_ptr->replace(pos, old_substring.size(), new_substring);
    pos += new_substring.size();
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
  ReplaceAll(&usage_section_, utility_name, "*UTILITY_DELIMITER*");
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
void DocPreprocessor::ReplaceElementWithRegularExpression(
    std::string *text_ptr,
    const regex &pattern,
    std::vector<std::string> *elements_ptr) {
  smatch match_result;
  while (regex_search(*text_ptr, match_result, pattern)) {
    auto element = match_result.str(1);
    // ` __id__ `. Notice that `__id__` is surrounded by spaces.
    string content = " __" + to_string(elements_ptr->size()) + "__ ";
    ReplaceAll(text_ptr, element, content);
    elements_ptr->push_back(element);
  }
}

// modify `text_`.
vector<string> DocPreprocessor::ReplaceSpeicalElement() {
  vector<string> elements;
  // process operands after `--`.
  regex operands_pattern("(-- (.|\n)*?)(\\s*)(\\*UTILITY_DELIMITER\\*)");
  // append delimiter in order to catch special operands of the last line.
  usage_section_ += "*UTILITY_DELIMITER*";
  ReplaceElementWithRegularExpression(
      &usage_section_,
      operands_pattern,
      &elements);
  // remove previous added dilimiter.
  usage_section_ = regex_replace(
      usage_section_,
      regex("\\*UTILITY_DELIMITER\\*$"),
      "");
  // process <argument>.
  regex argument_pattern("(<.*?>)");
  ReplaceElementWithRegularExpression(
      &usage_section_,
      argument_pattern,
      &elements);
  ReplaceElementWithRegularExpression(
      &options_section_,
      argument_pattern,
      &elements);
  // process default value.
  regex default_value_pattern("(\".*?\")");
  ReplaceElementWithRegularExpression(
      &options_section_,
      default_value_pattern,
      &elements);
  // update `text_`.
  text_ = usage_section_ + options_section_;
  return elements;
}

// restore `text_`.
void DocPreprocessor::RestoreSpeicalElement(
    const std::vector<std::string> &elements) {
  int index = 0;
  for (const string &element : elements) {
    string content = "__" + to_string(index) + "__";
    ReplaceAll(&text_, content, element);
    ++index;
  }
}

void DocPreprocessor::DisambiguateByInsertSpace() {
  // preprocess.
  auto elements = ReplaceSpeicalElement();
  vector<string> keywords = {
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
  // first, insert spaces.
  for (const string &keyword : keywords) {
    ReplaceAll(&text_, keyword, " " + keyword + " ");
  }
  // collapse spaces.
  text_ = regex_replace(
      text_,
      regex("\\s+"),
      " ");
  if (text_.back() == ' ') {
    text_.pop_back();
  }
  // postprocess.
  RestoreSpeicalElement(elements);
}

void DocPreprocessor::ExtractAndProcessUsageSection() {
  if (ExtractSection("Usage", text_, &usage_section_)) {
    ReplaceUtilityName();
  } else {
    // can't find usage section, which must exist.
    throw logic_error("ExtractAndProcessUsageSection");
  }
}

void DocPreprocessor::ExtractAndProcessOptionsSection() {
  if (ExtractSection("Options", text_, &options_section_)) {
    InsertDesDelimiter();
  } else {
    // Since options section is optional, if the program cannot find such
    // section, the program will return an empty string.
    options_section_ = "";
  }
}

string DocPreprocessor::PreprocessRawDoc(const string &raw_doc) {
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

string ParserProxy::PreprocessRawDoc(const string &raw_doc) {
  DocPreprocessor preprocessor;
  return preprocessor.PreprocessRawDoc(raw_doc);
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

void ParserProxy::PostProcessedAST(
    Doc::SharedPtr doc_node,
    OptionBindingRecorder *recorder_ptr,
    set<Token> *focused_elements_ptr) {
  // 1. remove duplicated nodes.
  StructureOptimizer structure_optimizer;
  doc_node->Accept(&structure_optimizer);
  // 2. process `--`.
  DoubleHyphenHandler double_dash_handler;
  doc_node->Accept(&double_dash_handler);
  // 3. handle ambiguous syntax.
  AmbiguityHandler ambiguity_handler(recorder_ptr);
  doc_node->Accept(&ambiguity_handler);
  // 4. collect focused elements.
  FocusedElementCollector focused_element_collector(recorder_ptr);
  doc_node->Accept(&focused_element_collector);
  *focused_elements_ptr = focused_element_collector.GetFocusedElement();
  // 5.TODO(huntzhan): issue #17.
  // 6. Remove bound arguments.
  auto bound_arguments = recorder_ptr->GetBoundArguments();
  BoundArgumentCleaner bound_argument_cleaner(bound_arguments);
  doc_node->Accept(&bound_argument_cleaner);
}

void ParserProxy::Parse(
    const std::string &doc,
    Doc::SharedPtr *doc_node_ptr,
    OptionBindingRecorder *recorder_ptr,
    set<Token> *focused_elements_ptr) {
  auto preprocessed_doc = PreprocessRawDoc(doc);
  ParseByBison(preprocessed_doc, doc_node_ptr, recorder_ptr);
  PostProcessedAST(*doc_node_ptr, recorder_ptr, focused_elements_ptr);
}

}  // namespace clidoc
