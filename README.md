# What is `clidoc`

Generally, `clidoc` is a source-to-source compiler, which is inspired by [docopt][].
`clidoc` analyses the syntax of command-line program(i.e. the synopsis of `ls`: `usage: ls [-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1] [file ...]` ), then genereates code of specific programming language that contains logic to process the input arguments of shell.

The phases of `clidoc` is as follow:

```                                                         
# front end.
doc --> | doc analyser | --> AST
                                
# back end.
        | cpp11 code gen logic  |
AST --> | python code gen logic | --> argument processing code
        | rust code gen logic   |
        | ...                   |
```

where the front end of `clidoc` would build up an AST for user defined `doc`(syntax of command-line program), and the back ends could generate argument processing code with respect to the AST. As you can see, `clidoc` plans to support code gen logic for multiple programming languages. But currently in `v0.1`, `clidoc` only support code gen of C++11.



# Requirements

The project is developed in C++11 with CMake. Known supported compilers:

* g++ 4.8.2 and higher.
* clang 3.3 and higher.

# Quick Start

A demo showing the usage of `clidoc` would be presented as follow. First of all, a CMake project is created, and the directory of `clidoc` is placed in the project:

```
➜  demo tree -L 1
.
├── CMakeLists.txt
└── clidoc

2 directories, 1 file
```

Thereafter, an example of `doc` is created:

```
# file `example_doc`.
Usage:
  example --hello-world
  example --output-file=<filename>
  example -n <names>...

Options:
  -n <names>  # bind -n with <names>
```

`example_doc` says that there is a program, named `example`, can be invoked by user with three kinds of input arguments:

* One with the form of `path/to/example --hello-world`
* Or with the form of `path/to/example --output-file=test.cc`
* Or with the form of `path/to/example -n one or more arguments`.

`example_doc` would be analysed by `clidoc`, but before that, we should create a `main.cc` for generating the program `example`:

```c++
// main.cc
#include <iostream>
#include "clidoc/cpp11.h"

int main(int argc, char *argv[]) {
  clidoc::ParseArguments(argc, argv);

  std::cout << std::boolalpha
            << "--hello-world: "
            << clidoc::boolean_outcome["--hello-world"]
            << std::endl;

  std::cout << "--output-file: "
            << clidoc::string_outcome["--output-file"]
            << std::endl;

  std::cout << "-n: ";
  for (const auto &value : clidoc::string_list_outcome["-n"]) {
    std::cout << value << " ";
  }
  std::cout << std::endl;
}
```

where `#include "clidoc/cpp11.h"` includes predefined C++11 interface of `clidoc`. `clidoc::ParseArguments(argc, argv);` would analyse input arguments and store the result into three `std::map` variable: `clidoc::boolean_outcome`, `clidoc::string_outcome` and `clidoc::string_list_outcome`.

Furthermore, we need to make connection between `example_doc` and `main.cc`. It's pretty easy to do that in CMake, by a  function `clidoc_cpp11_handler`:

```cmake
cmake_minimum_required (VERSION 2.8)
project (demo CXX)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -std=c++11")

add_subdirectory (clidoc)
include_directories (${clidoc_SOURCE_DIR}/include)

add_executable (example main.cc)
clidoc_cpp11_handler (example ${demo_SOURCE_DIR}/example_doc)
```

And then, use following command to compile the demo:

```
➜  demo tree -L 1
.
├── CMakeLists.txt
├── clidoc
├── example_doc
└── main.cc

1 directory, 3 files
➜  demo mkdir build       
➜  demo cd build 
➜  build cmake ..
-- The CXX compiler identification is AppleClang 6.0.0.6000056
...
-- Configuring done
-- Generating done
-- Build files have been written to: /Users/haoxun/Project/demo/build
➜  build make
Scanning dependencies of target clidoc_code_gen_logic
[  3%] Building CXX object clidoc/src/cpp11/CMakeFiles/clidoc_code_gen_logic.dir/code_gen_logic.cc.o
Linking CXX static library libclidoc_code_gen_logic.a
[  3%] Built target clidoc_code_gen_logic
...
[ 34%] Generating example_doc-cpp11.cc
Scanning dependencies of target example_doc
[ 37%] Building CXX object CMakeFiles/example_doc.dir/example_doc-cpp11.cc.o
Linking CXX static library libexample_doc.a
...
➜  build ls
CMakeCache.txt       Makefile             cmake_install.cmake  example_doc-cpp11.cc
CMakeFiles           clidoc               example              libexample_doc.a
```

Finally, run the `example`:

```
➜  build ./example 
Usage:
  example --hello-world
  example --output-file=<filename>
  example -n <names>...

Options:
  -n <names>  bind -n with <names>

➜  build ./example --hello-world
--hello-world: true
--output-file: 
-n: 
➜  build ./example --output-file=test.cc  
--hello-world: false
--output-file: test.cc
-n: 
➜  build ./example -n a b c d e
--hello-world: false
--output-file: 
-n: a b c d e 
➜  build ./example --error-case
Usage:
  example --hello-world
  example --output-file=<filename>
  example -n <names>...

Options:
  -n <names>  bind -n with <names>


```

# Syntax of `doc`

In this section, the rules of writing `doc` would be presented in bottom-up method. We will tall about the preprocessing strategy of `doc`, then the rules to define lexems in `doc`, and finally the context-free grammer to build a parser tree of `doc`. Currently the parsing tree of `doc` is built up with Flex/Bison generated code.

Similiar to `docopt`, syntax of `doc` is designed to be [POSIX.1-2008][] compatiable. 

## Preprocessing Strategy

Before scanning and parsing, the `doc` needs to be preprocessed for two main reasons:

* remove irrelevant textual elements, such as comments.
* disambiguate some textual elements for lexical analysis.

Generally, there are five steps in `doc` preprocessing:

1. Remove comment, empty line and tailing whitespace.
1. Detect utility name in usage section, then replace it with `K_UTILITY_DELIMITER`.
1. Insert `K_DESC_DELIMITER` after each newline character in option section.
1. Disambiguate textual elements by inserting spaces to both sides of every keyword.

Related details will be covered in following sub-sections.

### Structure of `doc`

A `doc` file is composed of two sections:

1. The `Usage Section`, which is **required to exist**.
2. The `Options Section`, which is **optional**.

`Usage Section` starts with the string `Usage:`(case-insensitive), followed by one of more utility descriptions. `Options Section` starts with the string `Options:`(case-insensitive), followed by one or more option bindings. Here's a simple example mentioned in `Quick Start`:

```
# begin of `Usage Section`.
Usage:
  example --hello-world
  example --output-file=<filename>
  example -n <names>...
# end of `Usage Section`.

# begin of `Options Section`.
Options:
  -n <names>  # bind -n with <names>
# end of `Options Section`.
```

### Comment

Comments would be removed at the first step of preprocessing. A comment starts with `#` and ends with newline character, just like Pyhton.

### Utility Name

`clidoc` would search for the first non-spaces textual element after `Usage:`(i.e. the `example` in previous example). Such element is treated as the utility name of the program. Hence, you can name the utility name of your program whatever you can think of, as long as it does not contain any space.

For disambigutation, `clidoc` will replace all appearances of utility name in `Usage Section` with `K_UTILITY_DELIMITER`.

### Option Bindings

Each non-empty line of `Option Section` is regarded as an option binding, which defines:

* the relation of option and option argument.
* the synonym form of option.
* the default value of option.

Syntax of option binding will be covered later. For now, the preprocessor only needs split up lines of `Option Section`, by inserting `K_DESC_DELIMITER` after each newline character.

### Insert Spaces

For the reason we will discuss in `Lexical Rules` section, `clidoc` must insert spaces to both sides of some keywords for disambiguation. Related keywords are as followed:

```
K_L_PARENTHESIS      "("
K_R_PARENTHESIS      ")"
K_L_BRACKET          "["
K_R_BRACKET          "]"
K_EXCLUSIVE_OR       "|"
K_ELLIPSES           "..."
K_EQUAL_SIGN         "="
K_OPTIONS            "options"
K_UTILITY_DELIMITER  "*UTILITY_DELIMITER*"
K_DESC_DELIMITER     "*DESC_DELIMITER*"
```

Notice that textual elements follows `--`, or elements quoted by angle brackets(`<`, `>`), or elements quoted by double quotes(`"`, `"`), will not be inserted spaces. Meaning of these elements will be explained later.

## Lexical Rules

comming soon.

```
DIGIT                [0-9]
LOWERCASE            [a-z]
UPPERCASE            [A-Z]
NON_SPACE            [^ \t\r\n\v\f]
HYPHEN               "-"
LOWERCASE_DIGIT      {LOWERCASE}|{DIGIT}
UPPERCASE_DIGIT      {UPPERCASE}|{DIGIT}
ALNUM                {LOWERCASE}|{UPPERCASE}|{DIGIT}

K_L_PARENTHESIS      "("
K_R_PARENTHESIS      ")"
K_L_BRACKET          "["
K_R_BRACKET          "]"
K_EXCLUSIVE_OR       "|"
K_ELLIPSES           "..."
K_USAGE_COLON        (?i:usage:)
K_OPTIONS_COLON      (?i:options:)
K_DEFAULT_COLON      (?i:default:)
K_OPTIONS            "options"
K_UTILITY_DELIMITER  "*UTILITY_DELIMITER*"
K_DESC_DELIMITER     "*DESC_DELIMITER*"

K_DOUBLE_HYPHEN      "--"
K_EQUAL_SIGN         "="

POSIX_OPTION         {HYPHEN}{ALNUM}
GROUPED_OPTIONS      {HYPHEN}{ALNUM}{2,}
GNU_OPTION           {HYPHEN}{2}{ALNUM}({ALNUM}|{HYPHEN})+

ARGUMENT_FORM_ONE    <[^>\n]+>
ARGUMENT_FORM_TWO    {UPPERCASE_DIGIT}({UPPERCASE_DIGIT}|{HYPHEN})*
ARGUMENT             {ARGUMENT_FORM_ONE}|{ARGUMENT_FORM_TWO}

DEFAULT_VALUE        \"[^\n\"]*\"

COMMAND              {NON_SPACE}+
```

## Context-free Grammer of `doc

comming soon.

## Adjustment of AST

comming soon.

# Interface of C++11

comming soon.

# Versions

comming soon.



[docopt]: https://github.com/docopt/docopt "docopt creates beautiful command-line interfaces."
[POSIX.1-2008]: http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap12.html "IEEE Std 1003.1, 2013 Edition: 12. Utility Conventions"
