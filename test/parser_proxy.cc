
#include <string>
#include "gtest/gtest.h"
#include "parser_proxy.h"

using namespace clidoc;
using std::string;

TEST(parser_proxy, preprocess_remove_empty_line) {
  string input = ("line one\n"
                  "line two\n"
                  "  \t   \n"
                  "\n\n\n\n\n\n"
                  "\t\n\n\n\n\n"
                  "line three\n"
                  "\n");
  string expect = ("line one\n"
                   "line two\n"
                   "line three\n");
  EXPECT_EQ(expect, RawTextPreprocessor::RemoveEmptyLine(input));
}

TEST(parser_proxy, preprocess_extract_section_1) {
  string input = ("Usage:\n"
                  "   this is line one.\n"
                  "   this is line two.\n"
                  "Options:\n"
                  "   this is line one.\n"
                  "   this is line two.\n");
  string expect = ("Usage:\n"
                   "   this is line one.\n"
                   "   this is line two.\n");
  string output;
  EXPECT_TRUE(RawTextPreprocessor::ExtractSection("Usage", input, &output));
  EXPECT_EQ(expect, output);
}

TEST(parser_proxy, preprocess_extract_section_2) {
  string input = ("Usage  \t :\n"
                  "   this is line one.\n"
                  "   this is line two.\n"
                  "Options:\n"
                  "   this is line one.\n"
                  "   this is line two.\n");
  string expect = ("Usage:\n"
                   "   this is line one.\n"
                   "   this is line two.\n");
  string output;
  EXPECT_TRUE(RawTextPreprocessor::ExtractSection("usage", input, &output));
  EXPECT_EQ(expect, output);
}

TEST(parser_proxy, preprocess_extract_section_3) {
  string input = ("Usage:\n"
                  "   this is line one.\n"
                  "   this is line two.\n"
                  "Options:\n"
                  "   this is line one.\n"
                  "   this is line two.\n");
  string expect = ("Options:\n"
                   "   this is line one.\n"
                   "   this is line two.\n");
  string output;
  EXPECT_TRUE(RawTextPreprocessor::ExtractSection("options", input, &output));
  EXPECT_EQ(expect, output);
}

TEST(parser_proxy, preprocess_replace_utility_name) {
  string input = ("Usage:\n"
                  "   some_program.py -f --long command\n"
                  "   some_program.py [whatever] command2\n");
  string expect = ("Usage:\n"
                  "   *UTILITY_DELIMITER* -f --long command\n"
                  "   *UTILITY_DELIMITER* [whatever] command2\n");
  EXPECT_EQ(expect, RawTextPreprocessor::ExtractAndProcessUsageSection(input));
}

TEST(parser_proxy, preprocess_process_options_section) {
  string input = ("Options:\n"
                  "   this is line one.\n"
                  "   this is line two.\n");
  string expect = ("Options:\n"
                   "   this is line one.\n*DESC_DELIMITER*"
                   "   this is line two.\n*DESC_DELIMITER*");
  EXPECT_EQ(expect, 
            RawTextPreprocessor::ExtractAndProcessOptionsSection(input));
}
