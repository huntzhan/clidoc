
#include <string>
#include "gtest/gtest.h"
#include "parser_proxy.h"
#include "utils.h"

#include <iostream>
#include "process_logic.h"

using namespace clidoc;
using std::string;

TEST(parser_proxy, preprocess_remove_comment) {
  string input = ("  line one #  whatever.\n"
                  "line two on comment.\n"
                  "line three#test.\n\n");
  string expect = ("  line one \n"
                   "line two on comment.\n"
                   "line three\n\n");
  EXPECT_EQ(expect, DocPreprocessor::RemoveComment(input));
}

TEST(parser_proxy, preprocess_remove_empty_line) {
  string input = ("line one\n"
                  "\t line two\n"
                  "  \t   \n"
                  "\n\n\n\n\n\n"
                  "\t\n\n\n\n\n"
                  "     line three\n"
                  "\n");
  string expect = ("line one\n"
                   "line two\n"
                   "line three\n");
  EXPECT_EQ(expect, DocPreprocessor::RemoveEmptyLine(input));
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
  EXPECT_TRUE(DocPreprocessor::ExtractSection("Usage", input, &output));
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
  EXPECT_TRUE(DocPreprocessor::ExtractSection("usage", input, &output));
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
  EXPECT_TRUE(DocPreprocessor::ExtractSection("options", input, &output));
  EXPECT_EQ(expect, output);
}

TEST(parser_proxy, preprocess_replace_utility_name) {
  string input = ("Usage:\n"
                  "   some_program.py -f --long command\n"
                  "   some_program.py [whatever] command2\n");
  string expect = ("Usage:\n"
                   "   *UTILITY_DELIMITER* -f --long command\n"
                   "   *UTILITY_DELIMITER* [whatever] command2\n");
  EXPECT_EQ(expect, DocPreprocessor::ExtractAndProcessUsageSection(input));
}

TEST(parser_proxy, preprocess_process_options_section) {
  string input = ("Options:\n"
                  "   this is line one.\n"
                  "   this is line two.\n");
  string expect = ("Options:\n"
                   "   this is line one.\n*DESC_DELIMITER*"
                   "   this is line two.\n*DESC_DELIMITER*");
  EXPECT_EQ(expect, 
            DocPreprocessor::ExtractAndProcessOptionsSection(input));
}

TEST(parser_proxy, preprocess_all_in_one) {
  string input = ("Usage  :\n"
                  "   some_program.py this is line one.\n"
                  "     # brbrbr.\n"
                  "   some_program.py this is line two.\n"
                  "\n\t \n\n"
                  "Options \t:\n"
                  "   this is line one.\n"
                  "   this is line two.\n"
                  "\n\n\n");

  string expect = ("Usage:\n"
                   "*UTILITY_DELIMITER* this is line one.\n"
                   "*UTILITY_DELIMITER* this is line two.\n"
                   "Options:\n"
                   "this is line one.\n*DESC_DELIMITER*"
                   "this is line two.\n*DESC_DELIMITER*");
  ParserProxy proxy;
  EXPECT_EQ(expect, proxy.PreprocessRawDoc(input));
}

TEST(parser_proxy, DISABLED_simple) {
  string input = ("Usage:\n"
                  "   some_program.py -c <some arg>\n"
                  "   some_program.py --long=SOMEARG\n");

  ParserProxy proxy;
  auto preprocess_doc = proxy.PreprocessRawDoc(input);
  Doc::SharedPtr doc_ptr;
  OptionBindingRecorder recorder;
  proxy.ParseByBison(preprocess_doc, &doc_ptr, &recorder);
  
  StructureOptimizer visitor;
  doc_ptr->Accept(&visitor);
  std::cout << doc_ptr->ToString(0) << std::endl;
}
