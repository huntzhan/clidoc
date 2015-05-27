#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include "clidoc/ast/ast_build.h"
#include "clidoc/codegen/cpp11_codegen.h"
#include "clidoc/codegen/filesystem.h"
#include "clidoc/codegen/python_codegen.h"
#include "clidoc/cpp11.h"

using std::cout;
using std::cerr;
using std::endl;
using std::exit;
using std::ifstream;
using std::map;
using std::ofstream;
using std::ostringstream;
using std::set;
using std::string;

void ConstructCodegenInfo(
    const string &synopsis_path, clidoc::CodegenInfo *info_ptr) {
  // load user defined doc.
  ifstream fin(synopsis_path);
  if (!fin.is_open()) {
    cout << "Invalid Doc Path." << endl;
    exit(1);
  }
  ostringstream ostrm;
  ostrm << fin.rdbuf();
  // build the AST.
  info_ptr->Prepare(ostrm.str());
}

void GenerateCpp11SourceCode(
    const string &synopsis_path,
    const string &output_file_name) {
  clidoc::CodegenInfo info;
  ConstructCodegenInfo(synopsis_path, &info);
  // code gen.
  ofstream fout(output_file_name);
  fout << clidoc::Cpp11Codegen(info);
  fout.close();
}

void GenerateCpp11CMakeProject(
    const string &synopsis_path,
    const string &output_dir) {

  clidoc::CodegenInfo info;
  ConstructCodegenInfo(synopsis_path, &info);

  clidoc::CopyDirectory(
      "cpp11/project_template",
      output_dir,
      clidoc::RELATIVE_TO_RESOURCE_DIR);
  clidoc::CopyDirectory(
      "include/clidoc/ast",
      clidoc::AppendPath(output_dir, "include/clidoc/ast"),
      clidoc::RELATIVE_TO_RESOURCE_DIR);
  clidoc::CopyFile(
      "src/ast/smart_ptr_interface.cc",
      clidoc::AppendPath(output_dir, "src/smart_ptr_interface.cc"),
      clidoc::RELATIVE_TO_RESOURCE_DIR);

  string filename = clidoc::AppendPath(output_dir, "src/codegen.cc");
  ofstream fout(filename.c_str());
  fout << clidoc::Cpp11Codegen(info);
  fout.close();
}


void GeneratePythonCode(
    const string &synopsis_path,
    const string &output_file_name) {
  clidoc::CodegenInfo info;
  ConstructCodegenInfo(synopsis_path, &info);
  // code gen.
  ofstream fout(output_file_name);
  fout << clidoc::PythonCodegen(info);
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

void Debug(const string &synopsis_path) {
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

  clidoc::CodegenInfo info;
  ConstructCodegenInfo(synopsis_path, &info);

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

  auto mode          = clidoc::string_outcome["--mode"];
  auto synopsis_path = clidoc::string_outcome["<synopsis>"];
  auto output_hint   = clidoc::string_outcome["<output_hint>"];

  if (clidoc::boolean_outcome["--list-mode"]) {
    ListMode();
    return 0;
  }
  if (clidoc::boolean_outcome["--debug"]) {
    Debug(synopsis_path);
    return 0;
  }

  auto function = MODE_FUNCTION_MAPPING.at(mode);
  function(synopsis_path, output_hint);
}
