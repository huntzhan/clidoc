#include "clidoc/ast/parser_proxy.h"

#include <cstdlib>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "boost/xpressive/xpressive_dynamic.hpp"
#include "clidoc/ast/ast_nodes.h"
#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/ast/generated_scanner.h"
#include "clidoc/ast/generated_parser.h"
#include "clidoc/ast/option_record.h"
#include "clidoc/ast/string_utils.h"

namespace re = boost::xpressive;

using std::back_inserter;
using std::istringstream;
using std::ostringstream;
using std::set;
using std::string;
using std::to_string;
using std::vector;

namespace clidoc {

void WrongUsageSectionFormatLogging() {
  std::cout << "can't find usage section." << std::endl;
  std::exit(1);
}

void DocPreprocessor::RemoveComment() {
  // must NOT remove the tailing NEWLINE character.
  re::sregex pattern = re::sregex::compile("#[^\r\n]*");
  text_ = re::regex_replace(text_, pattern, "");
}

void DocPreprocessor::RemoveEmptyLine() {
  re::sregex pattern = re::sregex::compile("(\n[ \t]*)+");
  text_ = re::regex_replace(text_, pattern, "\n");
}

std::string DocPreprocessor::GetUtilityName(const std::string &raw_doc) {
  re::sregex utility_name_pattern = re::sregex::compile(
      "usage:\\s*(\\S+)",
      re::regex_constants::icase);
  re::smatch match_result;
  bool found = re::regex_search(
      raw_doc,
      match_result,
      utility_name_pattern);
  if (!found) {
    WrongUsageSectionFormatLogging();
  }
  return match_result.str(1);
}

void DocPreprocessor::ReplaceUtilityName() {
  auto utility_name = GetUtilityName(usage_section_);
  StringUtils::ReplaceAll(
      utility_name, "*UTILITY_DELIMITER*", &usage_section_);
}

void DocPreprocessor::InsertDesDelimiter() {
  // insertion point:
  // Options: \n[NOT here]
  //   -x, -xxx # brbrbr. \n[HERE!]
  //   ...
  re::sregex options_section_name_pattern = re::sregex::compile(
      "options:\\s*",
      re::regex_constants::icase);
  re::smatch match_result;
  // have no chance to fail.
  re::regex_search(
      options_section_, match_result, options_section_name_pattern);
  if (match_result.suffix().first == options_section_.end()) {
    // case happen if nothing is following the name of options section
    // ("Options:").
    options_section_ = "";
    return;
  }
  string processed_options_section = match_result.str();
  auto pos_iter = match_result.suffix().first;
  re::regex_replace(
      back_inserter(processed_options_section),
      pos_iter, options_section_.cend(),
      re::sregex::compile("\n"),
      "\n*DESC_DELIMITER* ");
  options_section_ = processed_options_section;
}

void DocPreprocessor::ExtractAndProcessUsageSection() {
  if (StringUtils::ExtractSection("Usage", text_, &usage_section_)) {
    ReplaceUtilityName();
  } else {
    // can't find usage section, which must exist.
    WrongUsageSectionFormatLogging();
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

  // update `text_`.
  text_ = usage_section_ + options_section_;
  return text_;
}

string DocPreprocessor::PreprocessRawDocForCodeGen(const string &raw_doc) {
  // Replace # and following space or tab.
  re::sregex pattern = re::sregex::compile("#[ \t]*");
  return re::regex_replace(raw_doc, pattern, "");
}

string ParserProxy::PreprocessRawDoc(const string &raw_doc) {
  DocPreprocessor preprocessor;
  return preprocessor.PreprocessRawDocForParsing(raw_doc);
}

void ParserProxy::ParseByBison(
    const string &preprocessed_doc,
    Doc::SharedPtr *doc_node_ptr,
    OptionBindingRecorder *option_recorder_ptr,
    UnboundArgumentDefaultValueRecorder *unbound_argument_recorder) {
  // setup scanner.
  ostringstream null_ostream;
  istringstream input_stream(preprocessed_doc);
  FlexGeneratedScanner lexer(&input_stream, &null_ostream);
  // init parser.
  BisonGeneratedParser bison_parser(
      &lexer, doc_node_ptr,
      option_recorder_ptr, unbound_argument_recorder);
  bison_parser.parse();

  // postprocess.
  option_recorder_ptr->ProcessCachedBindings();
  // free smart pointer cached.
  SPIStaticDataMember::FreeCache();
}

void ParserProxy::Parse(
    const std::string &doc,
    Doc::SharedPtr *doc_node_ptr,
    OptionBindingRecorder *option_recorder_ptr,
    UnboundArgumentDefaultValueRecorder *unbound_argument_recorder) {
  auto preprocessed_doc = PreprocessRawDoc(doc);
  ParseByBison(
      preprocessed_doc, doc_node_ptr,
      option_recorder_ptr, unbound_argument_recorder);
}

}  // namespace clidoc
