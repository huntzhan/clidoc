#ifndef SRC_AST_PARSER_PROXY_H_
#define SRC_AST_PARSER_PROXY_H_

#include <regex>
#include <string>
#include <vector>
#include <set>

#include "ast/ast_nodes.h"
#include "ast/option_record.h"
#include "gtest/gtest_prod.h"

namespace clidoc {

struct StringUtils {
  static bool ExtractSection(
      const std::string &section_name,
      const std::string &text,
      std::string *output);
  static void ReplaceAll(
      const std::string &old_substring,
      const std::string &new_substring,
      std::string *text_ptr);
  static void InsertSpace(
      const std::vector<std::string> &regex_strings,
      const std::vector<std::string> &keywords,
      std::string *text_ptr);

 private:
  static void ReplaceElementWithMark(
      const std::regex &pattern,
      std::string *text_ptr,
      std::vector<std::string> *elements_ptr);
  static std::vector<std::string> ReplaceSpeicalElement(
      const std::vector<std::string> &regex_strings,
      std::string *text_ptr);
  static void RestoreSpeicalElement(
      const std::vector<std::string> &elements,
      std::string *text_ptr);
};

class DocPreprocessor {
 public:
  std::string PreprocessRawDocForParsing(const std::string &raw_doc);
  std::string PreprocessRawDocForCodeGen(const std::string &raw_doc);

 private:
  // gtest related.
  FRIEND_TEST(DocPreprocessorTest, RemoveComment);
  FRIEND_TEST(DocPreprocessorTest, RemoveEmptyLine);
  FRIEND_TEST(DocPreprocessorTest, ReplaceUtilityName);
  FRIEND_TEST(DocPreprocessorTest, ExtractAndProcessOptionsSection);

  void ExtractAndProcessUsageSection();
  void ExtractAndProcessOptionsSection();

  // 1. Remove comment and empty line and tailing whitespace.
  void RemoveComment();
  void RemoveEmptyLine();
  // 2. Detect utility name in usage section, then replace it with
  // K_UTILITY_DELIMITER.
  void ReplaceUtilityName();
  // 3. Insert K_DESC_DELIMITER brefore each NEWLINE in option section.
  void InsertDesDelimiter();
  // 4. Insert space to both sides of every keyword.
  void DisambiguateByInsertSpace();

  std::string text_;
  std::string usage_section_;
  std::string options_section_;
};

class ParserProxy {
 public:
  void Parse(
      const std::string &doc,
      Doc::SharedPtr *doc_node_ptr,
      OptionBindingRecorder *recorder_ptr);

 private:
  FRIEND_TEST(ParserProxyTest, PreprocessRawDoc);
  FRIEND_TEST(OptionBindingRecorderTest, RecordBinding);

  std::string PreprocessRawDoc(const std::string &raw_doc);
  void ParseByBison(
      const std::string &preprocessed_doc,
      Doc::SharedPtr *doc_node_ptr,
      OptionBindingRecorder *recorder_ptr);
};

}  // namespace clidoc
#endif  // SRC_AST_PARSER_PROXY_H_
