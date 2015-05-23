#include "clidoc/ast/string_utils.h"

#include <cstddef>
#include <string>
#include <vector>

#include "boost/regex.hpp"

namespace re = boost;

using std::size_t;
using std::string;
using std::to_string;
using std::vector;

namespace clidoc {

bool StringUtils::ExtractSection(
    const string &section_name,
    const string &text,
    string *output) {
  re::regex target_section_pattern(
      "(" + section_name + ")[ \t]*:",
      // case-insensitive.
      re::regex_constants::icase);
  re::regex next_section_pattern(
      "(\\w+?)[ \t]*:",
      re::regex_constants::icase);
  re::smatch match_result;
  auto pos_iter = text.cbegin();
  auto text_end_iter = text.cend();

  bool found = false;
  // search the begin of section.
  found = re::regex_search(
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
  found = re::regex_search(
      pos_iter, text_end_iter,
      match_result,
      next_section_pattern);

  auto section_end_iter = text_end_iter;
  if (found) {
    // ignroe `[default: "xxx"]`.
    bool found = re::regex_match(
        match_result.str(1),
        re::regex("default", re::regex_constants::icase));
    if (!found) {
      section_end_iter = match_result[0].first;
    }
  }

  // remove blank in section name.
  string extract_text(section_begin_iter, section_end_iter);
  *output = re::regex_replace(
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
    const string &regex_string,
    std::string *text_ptr,
    std::vector<std::string> *elements_ptr) {
  re::smatch match_result;
  while (re::regex_search(*text_ptr, match_result,
                      re::regex(regex_string))) {
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
  *text_ptr = re::regex_replace(*text_ptr, re::regex("\\s+"), " ");
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
        regex_string,
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

}  // namespace clidoc
