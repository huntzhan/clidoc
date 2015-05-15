#ifndef SRC_COMPLETION_COMPLETION_H_
#define SRC_COMPLETION_COMPLETION_H_

#include "ast_analyser.h"
#include "script_generator.h"

using std::string;

namespace clidoc{

void ParseScriptArguments(
    const string& doc_path,
    const string& output_file_name,
    bool is_manual);


} //namespace clidoc

#endif //SRC_COMPLETION_AST_ANALYSER_H_
