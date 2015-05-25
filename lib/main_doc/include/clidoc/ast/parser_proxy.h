#ifndef INCLUDE_CLIDOC_AST_PARSER_PROXY_H_
#define INCLUDE_CLIDOC_AST_PARSER_PROXY_H_

#include <string>

#include "gtest/gtest_prod.h"
#include "clidoc/ast/ast_nodes.h"
#include "clidoc/ast/option_record.h"

namespace clidoc {

class DocPreprocessor {
 public:
  std::string PreprocessRawDocForParsing(const std::string &raw_doc);
  std::string PreprocessRawDocForCodeGen(const std::string &raw_doc);
  std::string GetUtilityName(const std::string &raw_doc);

 private:
  // gtest related.
  FRIEND_TEST(DocPreprocessorTest, RemoveComment);
  FRIEND_TEST(DocPreprocessorTest, RemoveEmptyLine);
  FRIEND_TEST(DocPreprocessorTest, ReplaceUtilityName);
  FRIEND_TEST(DocPreprocessorTest, ExtractAndProcessOptionsSection);

  void ExtractAndProcessUsageSection();
  void ExtractAndProcessOptionsSection();

  // 1. Remove comment, empty line and tailing whitespace.
  void RemoveComment();
  void RemoveEmptyLine();
  // 2. Detect utility name in usage section, then replace it with
  // K_UTILITY_DELIMITER.
  void ReplaceUtilityName();
  // 3. Insert K_DESC_DELIMITER after each newline character in option section.
  void InsertDesDelimiter();

  std::string text_;
  std::string usage_section_;
  std::string options_section_;
};

class ParserProxy {
 public:
  void Parse(
      const std::string &doc,
      Doc::SharedPtr *doc_node_ptr,
      OptionBindingRecorder *option_recorder_ptr,
      UnboundArgumentDefaultValueRecorder *unbound_argument_recorder);

 private:
  FRIEND_TEST(ParserProxyTest, PreprocessRawDoc);
  FRIEND_TEST(OptionBindingRecorderTest, RecordBinding);

  std::string PreprocessRawDoc(const std::string &raw_doc);
  void ParseByBison(
      const std::string &preprocessed_doc,
      Doc::SharedPtr *doc_node_ptr,
      OptionBindingRecorder *option_recorder_ptr,
      UnboundArgumentDefaultValueRecorder *unbound_argument_recorder);
};

}  // namespace clidoc
#endif  // INCLUDE_CLIDOC_AST_PARSER_PROXY_H_
