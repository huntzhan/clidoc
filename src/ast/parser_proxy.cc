
#include <stdexcept>
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
#include "ast/string_utils.h"

using std::string;
using std::regex;
using std::smatch;
using std::regex_replace;
using std::regex_search;
using std::back_inserter;
using std::istringstream;
using std::ostringstream;
using std::vector;
using std::to_string;
using std::logic_error;
using std::set;

namespace clidoc {

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
