#ifndef SRC_PARSER_PROXY_H_
#define SRC_PARSER_PROXY_H_

#include <map>
#include <regex>
#include <string>
#include <vector>
#include "utils.h"

#include "gtest/gtest_prod.h"

namespace clidoc {

struct RepresentativeOptionProperty {
  // mutators.
  void set_option_argument(const Token &option_argument);
  void set_default_value(const std::string &default_value);

  bool has_option_argument_ = false;
  bool has_default_value_ = false;
  Token option_argument_;
  std::string default_value_;
};

// Record different kinds of optino binding during parsing.
class OptionBindingRecorder {
 public:
  void RecordBinding(
      OptionBindingContainer::SharedPtr container_ptr,
      DefaultValue::SharedPtr default_value_ptr);
  void RecordBinding(const Token &option, const Token &option_argument);
  void ProcessCachedBindings();

  std::map<Token, Token> option_to_representative_option_;
  std::map<Token, RepresentativeOptionProperty>
      representative_option_to_property_;

 private:
  Token GetRepresentativeOption(
      OptionBindingContainer::SharedPtr container_ptr);
  Token GetBoundOptionArgument(
      const Token &representative_option,
      OptionBindingContainer::SharedPtr container_ptr);
  void CreateRepresentativeOptionProperty(
      const Token &representative_option,
      const Token &bound_option_argument,
      const Token &default_value);
  void UpdateRepresentativeOptionProperty(
      const Token &representative_option,
      const Token &bound_option_argument,
      const Token &default_value,
      RepresentativeOptionProperty *property_ptr);

  // store option -> option_argument binding when parsing usage section.
  std::map<Token, Token> option_to_option_argument_cache_;
};

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
 public:
  std::string PreprocessRawDoc(const std::string &raw_doc);
  void ParseByBison(
      const std::string &preprocessed_doc,
      Doc::SharedPtr *doc_ptr,
      OptionBindingRecorder *option_binding_recorder_ptr);
};

}  // namespace clidoc
#endif  // SRC_PARSER_PROXY_H_
