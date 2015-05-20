#!/bin/bash

CLIDOC_DIR="$(dirname $0)/.."

single_file_cpplint() {
  while read -r file; do
    cpplint --root="${CLIDOC_DIR}/src" --filter=-legal/copyright,-build/c++11,-runtime/string $file
    printf "\n=============================================\n"
  done
}

find "${CLIDOC_DIR}/src" "${CLIDOC_DIR}/include" -type f                   \
    \( -not -name "generated_*.cc" -and -not -name "generated_parser.h"    \
       -and -not -name "FlexLexer.h" \)                                    \
    -and \( -name "*.h" -or -name "*.cc" \)                                \
		| single_file_cpplint 
