#define STRINGIFY(x) TO_STRING(x)
#define TO_STRING(x) #x

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <streambuf>
#include <string>

#include "clidoc/cpp11.h"
#include "clidoc/ast/ast_build.h"
#include "clidoc/cpp11/code_gen_logic.h"

using std::cout;
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
    const string &output_hint) {
  auto ExecuteSystemCommand = [](ostringstream *ostrm_ptr) {
    // 1. run command.
    string command = ostrm_ptr->str();
    system(command.c_str());
    // 2. reset `ostrm`.
    ostrm_ptr->str("");
    ostrm_ptr->clear();
  };

  // append "/" to the end if necessary.
  const string output_dir_name =
      output_hint.back() == '/'? output_hint : output_hint + "/";

  clidoc::CodeGenInfo info;
  PrepareForCpp11(doc_path, &info);

  ostringstream ostrm;
  // 1. copy cmake project template.
  ostrm << "cp -r "
        << kBinaryDirPath << "src/files/src/cpp11/project_template "
        << output_dir_name;
  ExecuteSystemCommand(&ostrm);

  // 2. copy serveral source files of `ast`.
  ostrm << "cp -r "
        << kBinaryDirPath << "src/files/include/clidoc/ast "
        << output_dir_name << "include/clidoc";
  ExecuteSystemCommand(&ostrm);

  ostrm << "cp "
        << kBinaryDirPath << "src/files/src/ast/smart_ptr_interface.cc "
        << output_dir_name << "src/";
  ExecuteSystemCommand(&ostrm);

  // 3. generate cpp11 source file with respect to doc.
  string filename = output_dir_name + "src/codegen.cc";
  ofstream fout(filename.c_str());
  fout << clidoc::GenerateSource(info);
  fout.close();
}

const map<string, void (*)(const string&, const string&)>
MODE_FUNCTION_MAPPING = {
  {"cpp11_non_project", GenerateCpp11SourceCode},
  {"cpp11_cmake_project", GenerateCpp11CMakeProject},
};

void ListMode() {
  for (const auto &map_pair : MODE_FUNCTION_MAPPING) {
    cout << map_pair.first << endl;
  }
}

int main(int argc, char **argv) {
  clidoc::ParseArguments(argc, argv);

  if (clidoc::boolean_outcome["--list-mode"]) {
    ListMode();
    return 0;
  }

  auto mode        = clidoc::string_outcome["--mode"];
  auto doc_name    = clidoc::string_outcome["<doc_name>"];
  auto output_hint = clidoc::string_outcome["<output_hint>"];

  auto function = MODE_FUNCTION_MAPPING.at(mode);
  function(doc_name, output_hint);
}
