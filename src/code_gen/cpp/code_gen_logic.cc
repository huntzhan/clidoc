#include <string>
#include <set>
#include <sstream>

#include "code_gen/cpp/code_gen_logic.h"
#include "ast/ast_node_interface.h"

using std::string;
using std::set;
using std::ostringstream;
using std::endl;

namespace clidoc {

string GenerateSetOfToken(
    const std::string &variable,
    const std::set<Token> &elements) {
  ostringstream ostrm;
  // assign to `variable`.
  ostrm << variable << " = {" << endl;
  for (const Token &element : elements) {
    ostrm << "  " << element.ToString() << "," << endl;
  }
  ostrm << "};" << endl;
  return ostrm.str();
}

}  // namespace clidoc
