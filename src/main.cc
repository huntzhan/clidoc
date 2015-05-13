#define STRINGIFY(x) TO_STRING(x)
#define TO_STRING(x) #x

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <streambuf>
#include <string>

#include "clidoc/cpp11.h"
#include "clidoc/ast/ast_build.h"
#include "clidoc/cpp11/codegen_logic.h"
#include "clidoc/python/codegen_logic.h"

using std::cout;
using std::cerr;
using std::endl;
using std::exit;
using std::ifstream;
using std::istreambuf_iterator;
using std::map;
using std::ofstream;
using std::set;
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
  if (!fin.is_open()) {
    cout << "Invalid Doc Path." << endl;
    exit(0);
  }
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
  fout << clidoc::cpp11::GenerateSource(info);
  fout.close();
}

void GenerateCpp11CMakeProject(
    const string &doc_path,
    const string &output_hint) {
  auto ExecuteSystemCommand = [](ostringstream *ostrm_ptr) {
    // 1. run command.
    string command = ostrm_ptr->str();
    // ignore return value of `system` explicitly.
    if (system(command.c_str())) { /* empty */ }
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
  ostrm << "mkdir "
        << output_dir_name << "include/clidoc/ast";
  ExecuteSystemCommand(&ostrm);

  ostrm << "cp "
        // ast_node_interface.h
        << kBinaryDirPath
        << "src/files/include/clidoc/ast/ast_node_interface.h "
        // ast_nodes.h
        << kBinaryDirPath
        << "src/files/include/clidoc/ast/ast_nodes.h "
        // smart_ptr_interface.h
        << kBinaryDirPath
        << "src/files/include/clidoc/ast/smart_ptr_interface.h "
        // dst.
        << output_dir_name << "include/clidoc/ast";
  ExecuteSystemCommand(&ostrm);

  ostrm << "cp "
        << kBinaryDirPath << "src/files/src/ast/smart_ptr_interface.cc "
        << output_dir_name << "src/";
  ExecuteSystemCommand(&ostrm);

  // 3. generate cpp11 source file with respect to doc.
  string filename = output_dir_name + "src/codegen.cc";
  ofstream fout(filename.c_str());
  fout << clidoc::cpp11::GenerateSource(info);
  fout.close();
}


void GeneratePythonCode(
    const string &doc_path,
    const string &output_file_name) {
  clidoc::CodeGenInfo info;
  PrepareForCpp11(doc_path, &info);
  // code gen.
  ofstream fout(output_file_name);
  fout << clidoc::python::GenerateSource(info);
  fout.close();
}

const map<string, void (*)(const string&, const string&)>
MODE_FUNCTION_MAPPING = {
  {"cpp11_non_project", GenerateCpp11SourceCode},
  {"cpp11_cmake_project", GenerateCpp11CMakeProject},
  {"python", GeneratePythonCode},
};

void ListMode() {
  for (const auto &map_pair : MODE_FUNCTION_MAPPING) {
    cout << map_pair.first << endl;
  }
}

void Debug(const string &doc_path) {
  auto PrintSetOfToken = [](
      const string &section_name,
      const set<clidoc::Token> &token_set) {
    cout << "[" << section_name << "]" << endl;
    if (token_set.empty()) {
      cout << "None" << endl;
    }
    for (const auto &token : token_set) {
      cout << token.ToString() << endl;
    }
    cout << endl;
  };

  clidoc::CodeGenInfo info;
  PrepareForCpp11(doc_path, &info);

  cout << "[AST]" << endl;
  cout << info.doc_node_->ToString(0) << endl;

  PrintSetOfToken("Bound Options", info.bound_options_);
  PrintSetOfToken("Unbound Options", info.unbound_options_);
  PrintSetOfToken("Unbound Arguments", info.arguments_);
  PrintSetOfToken("OOM Bound Options", info.oom_bound_options_);
  PrintSetOfToken("OOM Unbound Arguments", info.oom_arguments_);
  PrintSetOfToken("Commands", info.commands_);

  cout << "[Default Value]" << endl;
  if (info.default_values_.empty()) {
    cout << "None" << endl;
  }
  for (const auto &map_pair : info.default_values_) {
    cout << map_pair.first.ToString()
         << ": \"" << map_pair.second << "\"" << endl;
  }
  cout << endl;

  cout << "[Option To Representative Option Mapping]" << endl;
  for (const auto &map_pair
       : info.option_recorder_.option_to_representative_option_) {
    cout << map_pair.first.ToString()
         << " --> " << map_pair.second.ToString() << endl;
  }
}

int main(int argc, char **argv) {
  clidoc::ParseArguments(argc, argv);

  auto mode        = clidoc::string_outcome["--mode"];
  auto doc_name    = clidoc::string_outcome["<doc_name>"];
  auto output_hint = clidoc::string_outcome["<output_hint>"];

  if (clidoc::boolean_outcome["--list-mode"]) {
    ListMode();
    return 0;
  }
  if (clidoc::boolean_outcome["--debug"]) {
    Debug(doc_name);
    return 0;
  }

  auto function = MODE_FUNCTION_MAPPING.at(mode);
  function(doc_name, output_hint);
}
