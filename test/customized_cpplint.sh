#!/usr/bin/env sh

single_file_cpplint() {
  while read -r file; do
    cpplint --filter=-legal/copyright,-build/c++11 $file
    printf "\n=============================================\n"
  done
}

find ../src -type f                                                        \
    \( -not -name "generated_*.cc" -and -not -name "generated_parser.h" \) \
    -and \( -name "*.h" -or -name "*.cc" \)                                \
		| single_file_cpplint 
