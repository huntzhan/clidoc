
#include <set>
#include <string>

#include "ast/ast_build.h"
#include "ast/parser_proxy.h"
#include "ast/ast_node_interface.h"

using std::set;
using std::string;

namespace clidoc {

void CodeGenInfo::Prepare(const std::string &raw_doc) {
  set<Token> focused_elements;
  DocPreprocessor doc_preprocessor;
  ParserProxy parser_proxy;
  // setup `doc_node`, `recorder`.
  parser_proxy.Parse(raw_doc, &doc_node, &recorder, &focused_elements);
  // setup `doc_text`.
  doc_text = doc_preprocessor.PreprocessRawDocForCodeGen(raw_doc);
  // setup focused elements/
  for (const Token &element : focused_elements) {
    if (recorder.IsBound(element)) {
      focused_bound_options.insert(element);
    } else if (element.type() == TerminalType::ARGUMENT) {
      focused_arguments.insert(element);
    } else {
      focused_unbound_options.insert(element);
    }
  }
}

}  // namespace clidoc
