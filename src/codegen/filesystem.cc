#define STRINGIFY(x) TO_STRING(x)
#define TO_STRING(x) #x

#include "clidoc/codegen/filesystem.h"
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include "boost/filesystem.hpp"

namespace fs = boost::filesystem;
using std::string;
using std::exit;
using std::cout;
using std::endl;
using std::ostringstream;
using std::ifstream;

const fs::path kResourceDir(STRINGIFY(CLIDOC_RESOURCE_DIR));

namespace clidoc {

fs::path PrepareSrc(const string &src, const FlagType flags) {
  fs::path src_path(src);
  if (RELATIVE_TO_RESOURCE_DIR & flags) {
    src_path = kResourceDir / src_path;
  }
  return src_path;
}

void CopyDirectory(const fs::path &src, const fs::path &dst) {
  for (auto iter = fs::directory_iterator(src);
       iter != fs::directory_iterator(); ++iter) {
    auto current_src_path = iter->path();
    auto current_dst_path = dst / current_src_path.filename();
    if (fs::is_directory(current_src_path)) {
      // create directory.
      fs::copy_directory(current_src_path, current_dst_path);
      // recursive copy.
      CopyDirectory(current_src_path, current_dst_path);
    }
    if (fs::is_regular_file(current_src_path)) {
      // copy file.
      fs::copy_file(current_src_path, current_dst_path);
    }
  }
}

void CopyFile(
    const string &src, const string &dst, const FlagType flags) {
  auto src_path = PrepareSrc(src, flags);
  fs::path dst_path(dst);
  // remove dst if exists.
  if (fs::exists(dst_path)) {
    fs::remove(dst_path);
  }
  fs::copy_file(src_path, dst_path);
}

void CopyDirectory(
    const string &src, const string &dst, const FlagType flags) {
  auto src_path = PrepareSrc(src, flags);
  fs::path dst_path(dst);
  // remove dst if exists.
  if (fs::exists(dst_path)) {
    fs::remove(dst_path);
  }
  fs::create_directory(dst_path);
  CopyDirectory(src_path, dst_path);
}

string LoadFileFromResource(const string &src) {
  auto src_path = PrepareSrc(src, RELATIVE_TO_RESOURCE_DIR);
  ostringstream ostrm;
  ifstream fin(src_path.generic_string());
  if (!fin.is_open()) {
    cout << src_path << " not exists." << endl;
    exit(0);
  }
  ostrm << fin.rdbuf();
  return ostrm.str();
}

}  // namespace clidoc
