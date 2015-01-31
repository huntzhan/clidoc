
#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>

#include "ast/ast_build.h"
#include "cpp11/code_gen_logic.h"

using std::ifstream;
using std::ofstream;
using std::string;
using std::cerr;
using std::endl;
using std::istreambuf_iterator;

const int kArgvSize = 4;
enum ArgvIndex {
  LANGUAGE = 1,
  INPUT_FILE_PATH,
  OUTPUT_FILE_NAME,
};

int main(int argc, char **argv) {
  if (argc != kArgvSize) {
    cerr << "Error arguments." << endl;
    return -1;
  }
  string language(argv[LANGUAGE]);
  ifstream fin(argv[INPUT_FILE_PATH]);
  ofstream fout(argv[OUTPUT_FILE_NAME]);

  // load user defined doc.
  const string doc(
      (istreambuf_iterator<char>(fin)),
      istreambuf_iterator<char>());
  // build the AST.
  clidoc::CodeGenInfo info;
  info.Prepare(doc);

  // language specific operations.
  if (language == "cpp11") {
    fout << clidoc::GenerateSource(info);
    fout.close();
  }
}
