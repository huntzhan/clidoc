#ifndef SRC_AST_PARSER_PROXY_H_
#define SRC_AST_PARSER_PROXY_H_

#include <regex>
#include <string>
#include <vector>

#include "ast/ast_nodes.h"
#include "ast/option_record.h"
#include "gtest/gtest_prod.h"

namespace clidoc {

class DocPreprocessor {
 public:
  std::string PreprocessRawDoc(const std::string &raw_doc);

 private:
  // gtest related.
  FRIEND_TEST(DocPreprocessorTest, RemoveComment);
  FRIEND_TEST(DocPreprocessorTest, RemoveEmptyLine);
  FRIEND_TEST(DocPreprocessorTest, ExtractSection);
  FRIEND_TEST(DocPreprocessorTest, ReplaceUtilityName);
  FRIEND_TEST(DocPreprocessorTest, ExtractAndProcessOptionsSection);

  // Exrtact a section targeted by `section_name` and formalize section name.
  static bool ExtractSection(
      const std::string &section_name,
      const std::string &text,
      std::string *output);
  static void ReplaceAll(
      std::string *text_ptr,
      const std::string &old_substring,
      const std::string &new_substring);

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
  void ReplaceElementWithRegularExpression(
      std::string *text_ptr,
      const std::regex &pattern,
      std::vector<std::string> *elements_ptr);
  std::vector<std::string> ReplaceSpeicalElement();
  void RestoreSpeicalElement(const std::vector<std::string> &elements);
  void DisambiguateByInsertSpace();

  std::string text_;
  std::string usage_section_;
  std::string options_section_;
};

class ParserProxy {
 private:
  FRIEND_TEST(ParserProxyTest, PreprocessRawDoc);
  FRIEND_TEST(OptionBindingRecorderTest, RecordBinding);

  std::string PreprocessRawDoc(const std::string &raw_doc);
  void ParseByBison(
      const std::string &preprocessed_doc,
      Doc::SharedPtr *doc_ptr,
      OptionBindingRecorder *option_binding_recorder_ptr);
  void PostProcessedAST(
      Doc::SharedPtr doc_ptr,
      OptionBindingRecorder *option_binding_recorder_ptr);
};

}  // namespace clidoc
#endif  // SRC_AST_PARSER_PROXY_H_
