#define STRINGIFY(x) TO_STRING(x)
#define TO_STRING(x) #x

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <streambuf>
#include <string>

#include "ast/ast_build.h"
#include "cpp11/code_gen_logic.h"

using std::cerr;
using std::endl;
using std::ifstream;
using std::istreambuf_iterator;
using std::map;
using std::ofstream;
using std::string;
using std::system;
using std::ostringstream;

// for locating resource files.
const string kBinaryDirPath =
    string(STRINGIFY(CMAKE_BINARY_DIR)).back() == '/'?
    STRINGIFY(CMAKE_BINARY_DIR) : STRINGIFY(CMAKE_BINARY_DIR) "/";
// for argument processing.
const int kArgvSize = 4;
enum ArgvIndex {
  MODE = 1,
  DOC_PATH,
  OUTPUT_HINT,
};

void PrepareForCpp11(const string &doc_path, clidoc::CodeGenInfo *info_ptr) {
  // load user defined doc.
  ifstream fin(doc_path);
  const string doc(
      (istreambuf_iterator<char>(fin)),
      istreambuf_iterator<char>());
  // build the AST.
  info_ptr->Prepare(doc);
}

void GenerateCpp11SourceCode(
    const string &doc_path, 
    const string &output_file_name) {
  clidoc::CodeGenInfo info;
  PrepareForCpp11(doc_path, &info);
  // code gen.
  ofstream fout(output_file_name);
  fout << clidoc::GenerateSource(info);
  fout.close();
}

void GenerateCpp11CMakeProject(
    const string &doc_path, 
    const string &output_dir_name) {
  // 1. run command.
  // 2. reset `ostrm`.
  auto ExecuteSystemCommand = [](ostringstream *ostrm_ptr) {
    string command = ostrm_ptr->str();
    system(command.c_str());
    ostrm_ptr->str("");
    ostrm_ptr->clear();
  };

  clidoc::CodeGenInfo info;
  PrepareForCpp11(doc_path, &info);

  ostringstream ostrm;
  // 1. copy cmake project template.
  ostrm << "cp -r "
        << kBinaryDirPath << "src/data/cmake_cpp11_code_gen_project "
        << output_dir_name;
  ExecuteSystemCommand(&ostrm);

  // 2. copy serveral source files of `ast`.
  ostrm << "cp -r "
        << kBinaryDirPath << "src/data/ast "
        << output_dir_name << "/src/";
  ExecuteSystemCommand(&ostrm);

  // 3. generate cpp11 source file with respect to doc.
  string filename = output_dir_name + "/src/codegen.cc";
  ofstream fout(filename.c_str());
  fout << clidoc::GenerateSource(info);
  fout.close();
}

const map<string, void (*)(const string&, const string&)>
MODE_FUNCTION_MAPPING = {
  {"cpp11_non_project", GenerateCpp11SourceCode},
  {"cpp11_cmake_project", GenerateCpp11CMakeProject},
};

int main(int argc, char **argv) {
  if (argc != kArgvSize) {
    cerr << "Error arguments." << endl;
    return -1;
  }
  string mode(argv[MODE]);
  auto function = MODE_FUNCTION_MAPPING.at(mode);
  function(argv[DOC_PATH], argv[OUTPUT_HINT]);
}
