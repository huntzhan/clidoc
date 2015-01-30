#ifndef SRC_AST_STRING_UTILS_H_
#define SRC_AST_STRING_UTILS_H_

#include <vector>
#include <string>

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
      const std::string &regex_string,
      std::string *text_ptr,
      std::vector<std::string> *elements_ptr);
  static std::vector<std::string> ReplaceSpeicalElement(
      const std::vector<std::string> &regex_strings,
      std::string *text_ptr);
  static void RestoreSpeicalElement(
      const std::vector<std::string> &elements,
      std::string *text_ptr);
};

}  // namespace clidoc
#endif  // SRC_AST_STRING_UTILS_H_
