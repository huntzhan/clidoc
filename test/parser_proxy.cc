
#include <string>
#include "gtest/gtest.h"
#include "parser_proxy.h"
#include "utils.h"

#include <iostream>
#include "process_logic.h"

using std::string;

namespace clidoc {

TEST(DocPreprocessorTest, RemoveComment) {
  DocPreprocessor preprocessor;
  preprocessor.text_ =
      "  line one #  whatever.\n"
      "line two on comment.\n"
      "line three#test.\n\n";
  string expect =
      "  line one \n"
      "line two on comment.\n"
      "line three\n\n";
  preprocessor.RemoveComment();
  EXPECT_EQ(expect, preprocessor.text_);
}

TEST(DocPreprocessorTest, RemoveEmptyLine) {
  DocPreprocessor preprocessor;
  preprocessor.text_ =
      "line one\n"
      "\t line two\n"
      "  \t   \n"
      "\n\n\n\n\n\n"
      "\t\n\n\n\n\n"
      "     line three\n"
      "\n";
  string expect =
      "line one\n"
      "line two\n"
      "line three\n";
  preprocessor.RemoveEmptyLine();
  EXPECT_EQ(expect, preprocessor.text_);
}

TEST(DocPreprocessorTest, ExtractSection) {
  string input1 =
      "Usage:\n"
      "   this is line one.\n"
      "   this is line two.\n"
      "Options:\n"
      "   this is line one.\n"
      "   this is line two.\n";
  string expect1 = 
      "Usage:\n"
      "   this is line one.\n"
      "   this is line two.\n";

  string input2 =
      "Usage  \t :\n"
      "   this is line one.\n"
      "   this is line two.\n"
      "Options:\n"
      "   this is line one.\n"
      "   this is line two.\n";
  string expect2 = 
      "Usage:\n"
      "   this is line one.\n"
      "   this is line two.\n";

  string input3 =
      "Usage:\n"
      "   this is line one.\n"
      "   this is line two.\n"
      "Options:\n"
      "   this is line one.\n"
      "   this is line two.\n";
  string expect3 =
      "Options:\n"
      "   this is line one.\n"
      "   this is line two.\n";

  string output1;
  EXPECT_TRUE(DocPreprocessor::ExtractSection("Usage", input1, &output1));
  EXPECT_EQ(expect1, output1);

  string output2;
  EXPECT_TRUE(DocPreprocessor::ExtractSection("usage", input1, &output2));
  EXPECT_EQ(expect2, output2);

  string output3;
  EXPECT_TRUE(DocPreprocessor::ExtractSection("options", input3, &output3));
  EXPECT_EQ(expect3, output3);
}

TEST(DocPreprocessorTest, ReplaceUtilityName) {
  DocPreprocessor preprocessor;
  preprocessor.usage_section_ =
      "Usage:\n"
      "   some_program+py -f --long command\n"
      "   some_program+py [whatever] command2\n";
  string expect =
      "Usage:\n"
      "   *UTILITY_DELIMITER* -f --long command\n"
      "   *UTILITY_DELIMITER* [whatever] command2\n";
  preprocessor.ReplaceUtilityName();
  EXPECT_EQ(expect, preprocessor.usage_section_);
}

TEST(DocPreprocessorTest, ExtractAndProcessOptionsSection) {
  DocPreprocessor preprocessor;
  preprocessor.text_ =
      "Options:\n"
      "   this is line one.\n"
      "   this is line two.\n";
  string expect =
      "Options:\n"
      "   this is line one.\n*DESC_DELIMITER*"
      "   this is line two.\n*DESC_DELIMITER*";
  preprocessor.ExtractAndProcessOptionsSection();
  EXPECT_EQ(expect, preprocessor.options_section_);
}

TEST(parser_proxy, preprocess_all_in_one) {
  string input = ("Usage  :\n"
                  "   some_program.py [-f] FILE [options] -- this\n"
                  "     # brbrbr.\n"
                  "   some_program.py (foo|bar) --long=<newline>\n"
                  "\n\t \n\n"
                  "Options \t:\n"
                  "   this is line one.\n"
                  "   this is line two.\n"
                  "\n\n\n");

  string expect = ("Usage:"
                   " *UTILITY_DELIMITER* [ -f ] FILE [ options ] -- this"
                   " *UTILITY_DELIMITER* ( foo | bar ) --long = <newline>"
                   " Options:"
                   " this is line one. *DESC_DELIMITER*"
                   " this is line two. *DESC_DELIMITER*");
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

}  // namespace clidoc
