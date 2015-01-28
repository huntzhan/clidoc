
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

int main(int argc, char **argv) {
  if (argc != 3) {
    cerr << "Error arguments." << endl;
    return -1;
  }
  // 1st arg should be abs path to doc file.
  ifstream fin(argv[1]);
  // 2nd arg should indicate the code gen language.
  string language(argv[2]);

  // load user defined doc.
  const string doc(
      (istreambuf_iterator<char>(fin)),
      istreambuf_iterator<char>());
  // build the AST.
  clidoc::CodeGenInfo info;
  info.Prepare(doc);

  // language specific operations.
  if (language == "cpp11") {
    ofstream fout("clidoc_cpp11_code_gen.cc");
    fout << clidoc::GenerateSource(info);
    fout.close();
  }
}
