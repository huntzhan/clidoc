# Introduction

`clidoc` analyses the **synopsis** of command-line program, then generates code of specific programming language that could be used to process shell inputed values.

Workflow of `clidoc` is as follow:

```                                                         
# front end.
doc --> | doc analyser | --> AST
                                
# back end.
        | cpp11 codegen  |
AST --> | python codegen | --> argument processing code
        | rust codegen   |
        | ...            |
```

where 

* the front end of `clidoc` would build up an AST(abstract syntax tree) according to user defined synopsis,
* and the back ends would generate argument processing code with respect to front end generated AST.

As you can see, `clidoc` plans to support codegen for multiple programming languages. Currently in `v0.2`, `clidoc` supports codegen of programming language C++11 and Python 2/3.

# Build

Known supported compilers: g++ 4.8.1 or higher; clang 3.3 or higher. 

To build `clidoc`, following dependencies is required:

* CMake 2.8 or higher.
* Boost 1.54 or higher.

## OS X

User of OS X could use `brew` to install dependencies:

```
brew install cmake
brew install boost
``` 

## Ubuntu

User of Ubuntu could use `apt-get` to install dependencies:

```
sudo apt-get install g++
sudo apt-get install cmake
sudo apt-get install libboost-all-dev 
```

Make sure the version of `apt-get` installed g++ is greater or equal to `4.8.1`. If not, try following steps:

```
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install g++-4.8
```

Make sure the version of `apt-get` installed boost libraries is greater or equal to `1.54`. If not, try following steps:

```
sudo add-apt-repository -y ppa:boost-latest/ppa
sudo apt-get update
sudo apt-get install libboost1.54-all-dev
``` 

## Build The Project [![Build Status](https://travis-ci.org/clidoc/clidoc.svg?branch=master)](https://travis-ci.org/clidoc/clidoc)


After all dependencies are installed, use bash script `scripts/clidoc-stuff.sh` to build the project:

```
git clone https://github.com/clidoc/clidoc.git
cd clidoc
./scripts/clidoc-stuff.sh build
```

After successfully building the project, a program named `clidoc_main` is created, located in `build/src`. Use following command to create an alias to `clidoc_main`:

```
alias clidoc="$(./scripts/clidoc-stuff.sh clidoc_main)"
```

After that, you can interact with alias `clidoc` in shell:

```
$ clidoc
Usage:
  clidoc_main --mode|-m <mode> <synopsis> <output-hint>
  clidoc_main --list-mode
  clidoc_main --debug <synopsis>

Options:
  --mode=<mode>, -m <mode>  Mode of codegen.
  <synopsis>                Filename of user defined synopsis.
  <output-hint>             String to guide codegen.
  --list-mode               List available modes.
  --debug                   List information for debugging.
```

In the following content, `clidoc` would refer to **the alias `clidoc`**.

# Writing Synopsis

## Basic Concepts

**Synopsis** defines the way to interact with a command-line program. As mentioned, `clidoc` reads user defined synopsis and generates code for that. Concepts related to synopsis will be discussed in this section.

Basic elements of synopsis are as follow:

* **POSIX_OPTION**: a form of option described in POSIX.1-2008. **POSIX_OPTION** is represented by a **single dash** followed by an **alnum**, such as `-a`, `-A`, `-2`.
* **GNU_OPTION**: a form of option described in GNU document. **GNU_OPTION** is represented by **double dash** followed by an **alnum**, and followed by one or more **alnum** or **single dash**, such as `--long`, `--long-option`, `--Long-Option`.
* **GROUPED_OPTIONS**: describe a group of options. **GROUPED_OPTIONS** is represented by a **single dash** followed by **two or more alnum**, such as `-abcDEF`.
* **ARGUMENT**: placeholder for some shell inputed value. **ARGUMENT** is represented by one of two forms: **uppercase digit**(abbreviation of "uppercase letter and digit") followed by one or more **uppercase digit** or **single dash**(i.e. `ARG`, `ARG1`, `ARG-1`, `SOME-ARG`), or, arbitrary string enclosed by **angle brackets**(i.e. `<arg>`, `<some arg>`).
* **COMMAND**: literally, it is "command". **COMMAND** is represented by **lowercase digit**(abbreviation of "lowercase letter and digit") followed by one or more **lowercase digit** or **single dash**, such as `add`, `brew-cask`.

Through "logical operators", user could define **logical relationship** of different basic elements, or **logical property** of a single basic element:

* **Parenthesis**(`(` and `)`): the logical relationship of  embraced elements is **LogicAnd**, meaning those elements are all required to be matched. By default, logical relationship of basic elements separated by whitespaces is `LogicAnd`, for instance, `(-a -b -c)` is equivalent to `-a -b -c`.
* **Vertical Bar**(`|`): the logical relationship of elements separated by vertical bar is **LogicXor**, meaning only one of those elements should be matched. Example: `--mode|-m`.
* **Square Brackets**(`[` and `]`): express the logical property of embraced element is optional(**LogicOptional**), in other words, the element is not required to be matched. Example: `[-g]`, `[--filename=FILENAME]`.
* **Ellipsis**(`...`): express the logical property of the element followed by ellipsis is repeatable(**LogicOneOrMore**), meaning that the element can be matched once or more. Example: `--filename=FILENAME...`.

User defined synopsis consists of two sections:

1. An **Usage Section**.
2. An **Options Section**(optional).

**Usage section** starts with the section name `Usage:`(case-insensitive `usage` plus a colon). The first non-empty string unit after `Usage:` would be recognized as **utility name**, that is, the name of command-line program. Usage section is composed of several **synopsis line**s, each synopsis line starts with an utility name and contains combinations of basic elements and logical operators. As an analogy, utility name acts like semicolon in C++, which is used to separated a full statement. Here is a example for demonstration:

```
Usage:
  utility_name -d [-a]
      [-c <option_argument>]
  utility_name[-a][-b][<operand>...]
```
where the utility name is `utility_name`, and the usage section is composed of two synopsis lines. Intuitively, the logical relationship of synopsis lines is **LogicXor**, meaning that synopsis lines of usage section could be matched mutual-exclusively.

**Options section** starts with the section name `Options:`(case-insensitive `option` plus a colon). Options section is composed of several **non-empty physical line**(a non-empty line ends with newline character), each non-empty physical line defines

* **binding relationship** between options, and between option and argument.
* **default value** of option and argument.

Binding relationship **between options** expresses that those options are equivalent. For example, in order to express "`-f` as the abbreviation of `--filename`", user could define something like:

```
Options:
  -f, --filename
```

Binding relationship **between option and argument** expresses that an option should be followed by an argument. For example, in order to express "`ARG1` is bound to `-a`, and `<arg2>` is bound to `--bar`", user could define something like:

```
Options:
  -f ARG1
  --bar=<arg2>  # or, --bar <arg2>
```

Combination of option-to-option binding and option-to-argument binding is allowed. For example, in order to express "`-f` as the abbreviation of `--filename`, where `FILE` should be bound to both `-f` and `--filename`", user could define something like:

```
Options:
  -f FILE, --filename=FILE
```

Note that:

* basic elements can be separated by whitespace(s) or comma(`,`). Hence, `-f FILE, --filename=FILE` is equivalent to `-f FILE --filename=FILE`, but the first expression is more readable.
* binding relationship between **GNU_OPTION** and **ARGUMENT** could be defined in usage section, through the form of `GNU_OPTION=ARGUMENT`.

Moreover, user could define default value of option and argument, by placing `[default: "some value"]` at the end of physical line, such as:

```
Options:
  -f FILE, --filename=FILE [default: "~/data.cfg"]
```

Comments are supported through hash character(`#`). A comment starts with `#` and ends with the newline character of current physical line(not including the newline character). Comments would be ignored during parsing user defined synopsis. Use synopsis of `clidoc` as an example:

```
$ cat scripts/main_doc 
Usage:
  clidoc_main --mode|-m <mode> <synopsis> <output-hint>
  clidoc_main --list-mode
  clidoc_main --debug <synopsis>

Options:
  --mode=<mode>, -m <mode>  # Mode of codegen.
  # <synopsis>                Filename of user defined synopsis.
  # <output-hint>             String to guide codegen.
  # --list-mode               List available modes.
  # --debug                   List information for debugging.
```

## Semantics Of Synopsis

Logical relationships between basic elements are as follow:

* **LogicAnd**
* **LogicXor**
* **LogicOr** (not mentioned yet)

Logical properties of a single basic element are as follow:

* **LogicOptional**
* **LogicOneOrMore**

Logical relationships and logical properties are the abstractions of logical operators, mapping of those were mentioned in above section(except **LogicOr**). Meanings of **LogicAnd**, **LogicXor** and **LogicOptional** are pretty intuitive, hence we will focus on explaining **LogicOneOrMore** and **LogicOr**.

Logical property **LogicOneOrMore** express the element followed by ellipsis(`...`) is repeatable. Consider following examples:

* `<arg1>...`
* `(-f <arg2>)...`
* `-f <arg2>...`

suppose `<arg1>` is not bound to any option, and `<arg2>` is bound to `-f`. 

As mentioned, **ARGUMENT** acts like a placeholder for shell inputed value(s): **ARGUMENT** without logical property **LogicOneOrMore** is a placeholder for a single value, while **ARGUMENT** with logical property **LogicOneOrMore** is a placeholder for multiple values. Hence, `<arg1>...`, an unbound **ARGUMENT** with logical property **LogicOneOrMore**,  defines a placeholder accepting multiple shell inputed values.

If an **ARGUMENT** is bound to an option(**POSIX_OPTION** or **GNU_OPTION**), logical property **LogicOneOrMore** of such **ARGUMENT**, if exists, would be added to that option. Therefore, `(-f <arg2>)...` and `-f <arg2>...` are equivalent, since both `-f` options have logical property **LogicOneOrMore**.

An OOM bound option(option is bound with an **ARGUMENT**, and has logical property **LogicOneOrMore**), could accept following forms of shell inputed values:

* `-f a b c`
* `-f a -f b -f c`
* `-f a,b,c`

above forms are equivalent. Those shell inputed values would leads to a same result, a list of strings contains "a", "b" and "c".

A **GROUPED_OPTIONS** is equivalent to a group of **POSIX_OPTION**s with logical relationship **LogicOr**, meaning that one or more expanded **POSIX_OPTION**s should be matched. For example, `-abc` is equivalent to `-a`, `-b` and `-c` with logical relationship **LogicOr**, and shell inputed values `-a`, `-b`, `-c`, `-a -b`, `-a -c`, `-b -c` and `-a -b -c` will be accepted.

For POSIX.1-2008 compatible, form of `-aboFILE` would be transformed to `-abo FILE` if **ARGUMENT** `FILE` is bound to **POSIX_OPTION** `-o`. For example:

```
Usage:
  utility_name -oFILE  # equivalent to "utility_name -o FILE"

Options:
  -o FILE
```

For POSIX.1-2008 compatible, basic elements after `--`, to the end of synopsis line, would be treated as **ARGUMENT**. For example:

```
Usage: utility_name -a -b -- -c command 
```
where `-c`, `command` would be as **ARGUMENT**s, instead of a **POSIX_OPTION** and a **COMMAND**.

# Codegen

`clidoc` reads user defined synopsis and generates code of  language specific shell inputed value processing logic. This section will present the rules of shell inputed value processing, the usage of `clidoc`, and the application interface of generated code.

## Rules Of Shell Inputed Value Processing

As mentioned in POSIX-1.2008, the order of shell inputed value  is unrelated to the order of elements defined in synopsis. For example, given synopsis `Usage: utility_name -a -b -c`, both shell inputed values `$ utility_name -a -b -c` and `$ utility_name -c -b -a` would be accepted. Since the order of shell inputed value is irrelevant, option(**POSIX_OPTION**, **GNU_OPTION**) and **COMMAND** plays a vital role in the interpretation of shell inputed value. 

`clidoc` generated code would process shell inputed values based on following rules:

1. If an bound option is detected, the value following such bound option, would be treated as bound **ARGUMENT**. 
2. If an OOM bound option is detected,
	1. and such option is the last option in shell inputed value with the same option name, i.e, the third `-f` in `-f a -f b -f c d e`. In this case, all values next to such option, ends with another option, would be treated as bound **ARGUMENT**s.
	2. otherwise, rule (1) would be applied.
3. If shell inputed value starts with **single dash** and followed by **two or more** characters, inputed value processing logic would try to split such value by looping over each characters. For example, given `-abcd`, processing logic would loop over `a`, `b`, `c`, `d`. Within the loop, a **POSIX_OPTION** will be assembled corresponding to current character(i.e. `a` -> `-a`). Rules of splitting logic:
	1. If such **POSIX_OPTION** is defined as **unbound option** in synopsis, record it and continue looping.
	2. If such **POSIX_OPTION** is defined as **bound option** in synopsis, and current character is **not** the last character, then record the option and treat remaining characters as  bound **ARGUMENT**. After that, stop looping.
	3. If such **POSIX_OPTION** is defined as **bound option** in synopsis, and current character is the last character, then record the option and treat the next shell inputed value as bound **ARGUMENT**. After that, stop looping.
	4. otherwise, the splitting logic would fail, and current shell inputed value would be treated as candidate of **ARGUMENT**.
4. If shell inputed value starts with **double dash**, then the processing logic would try to match inputed value with pattern `GNU_OPTION=ARGUMENT`. For instance, `--filename=~/data.cfg` would be a perfect match.
	1. If detected **GNU_OPTION** is defined as **bound option** in synopsis, then the string next to equal sign(`=`) would be treated as bound **ARGUMENT**.
	2. otherwise, current shell inputed value would be treated as candidate of **ARGUMENT**.
5. If `--` is detected, all shell inputed values after `--` would be treated as candidates of **ARGUMENT**.

Outcomes would be generated after processing shell inputed values. There are three types of outcomes:

* **boolean** outcome: representing the processing result of **COMMAND**s and **unbound option**s.
* **string** outcome: representing the processing result of **bound option**s and **unbound ARGUMENT**s
* **string list** outcome: representing the processing result of **OOM bound option**s and **OOM unbound ARGUMENT**s

Generated code should implement a mapping data structure so that the outcomes could be read through the name of basic elements. Note that only the **representative option**(the first occurrent **GNU_OPTION**) in a options binding relationship would be considered as the index of outcome, since options in the same binding relationship would be treated as synonyms.

## Usage of `clidoc`

Synopsis of `clidoc` is as followed:

```
Usage:
  clidoc_main --mode|-m <mode> <synopsis> <output-hint>
  clidoc_main --list-mode
  clidoc_main --debug <synopsis>

Options:
  --mode=<mode>, -m <mode>  Mode of codegen.
  <synopsis>                Filename of user defined synopsis.
  <output-hint>             String to guide codegen.
  --list-mode               List available modes.
  --debug                   List information for debugging.
```

Currently `clidoc` supports three codegen modes:

* `cpp11_cmake_project`: generate a C++11 CMake project.
* `cpp11_non_project`: generate a C++11 source file. This mode supports integration of CMake.
* `python`: generate a Python 2/3 compatible file.

Example of usage:

```
$ clidoc --mode=cpp11_cmake_project scripts/main_doc main_doc
$ clidoc --mode=python scripts/main_doc main_doc.py
```

## C++11 Codegen

Interface of C++11 generated code is defined in a header file `clidoc/cpp11.h`, which is simple and self-explanatory:

```c++
#ifndef SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_CPP11_H_
#define SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_CPP11_H_

#include <cstdint>
#include <map>
#include <vector>
#include <string>

namespace clidoc {

using FlagType = std::uint32_t;

const FlagType SYSTEM_EXIT_OFF = 1 << 0;
const FlagType PRINT_DOC_OFF   = 1 << 1;
const FlagType GUIDELINE_8_OFF = 1 << 2;

// store outcomes.
extern std::map<std::string, bool> boolean_outcome;
extern std::map<std::string, std::string> string_outcome;
extern std::map<std::string, std::vector<std::string>> string_list_outcome;

// input argument processing interface.
void ParseArguments(const int &argc, const char *const *argv);
bool ParseArguments(const int &argc, const char *const *argv,
                    const FlagType &flags);

}  // namespace clidoc

#endif  // SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_CPP11_H_
``` 

There are two ways to generate and use C++11 code:

* Include the project directory of "clidoc" to your CMake project, then invoke a CMake function `clidoc_cpp11_handler`.
* Use `clidoc` to generate a CMake project, then include the generated directory(project) to your CMake project.

### Placing the project directory "clidoc" in your CMake project

A demo will be presented for demonstration. First of all, a CMake project is created, and the directory of "clidoc" is placed in the project:

```
$ tree -L 1
.
├── CMakeLists.txt
└── clidoc

2 directories, 1 file
```

Then, a synopsis for demonstration is created:

```
# file `example_doc`.
Usage:
  example --hello-world
  example --output-file=<filename>
  example -n <names>...

Options:
  -n <names>  # bind -n with <names>
```

`example_doc` says that the program `example` can be invoked by three forms of shell inputed values:

* `path/to/example --hello-world`
* `path/to/example --output-file=test.cc`
* `path/to/example -n one or more arguments`.

`example_doc` would be analysed by `clidoc`. But before that, we should implement a C++ source file `main.cc` for generating the program `example`:

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

where

* `#include "clidoc/cpp11.h"` includes predefined C++11 interface of `clidoc`.
* `clidoc::ParseArguments(argc, argv);` analyses shell input values and store the result into three `std::map` variable: `clidoc::boolean_outcome`, `clidoc::string_outcome` and `clidoc::string_list_outcome`.

Furthermore, connection between `example_doc` and `main.cc` should be established(i.e. codegen of `example_doc`, compile generated code, and linkage stuff). It's pretty easy to do that in CMake, by invoking a function `clidoc_cpp11_handler`:

```cmake
cmake_minimum_required (VERSION 2.8)
project (demo CXX)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -std=c++11")

add_subdirectory (clidoc)
include_directories (${CLIDOC_CPP11_INCLUDE_PATHS})

add_executable (example main.cc)
clidoc_cpp11_handler (example ${demo_SOURCE_DIR}/example_doc)
```

Afterward, use following commands to compile the demo:

```
$ tree -L 1
.
├── CMakeLists.txt
├── clidoc
├── example_doc
└── main.cc

1 directory, 3 files
$ mkdir build       
$ cd build 
$ cmake ..
-- The CXX compiler identification is AppleClang 6.0.0.6000056
...
-- Configuring done
-- Generating done
-- Build files have been written to: /Users/haoxun/Project/demo/build
$ make
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
$ ls
CMakeCache.txt       Makefile             cmake_install.cmake  example_doc-cpp11.cc
CMakeFiles           clidoc               example              libexample_doc.a
```

Finally, run the program `example`:

```
$ ./example 
Usage:
  example --hello-world
  example --output-file=<filename>
  example -n <names>...

Options:
  -n <names>  bind -n with <names>

$ ./example --hello-world
--hello-world: true
--output-file: 
-n: 
$ ./example --output-file=test.cc  
--hello-world: false
--output-file: test.cc
-n: 
$ ./example -n a b c d e
--hello-world: false
--output-file: 
-n: a b c d e 
$ ./example --error-case
Usage:
  example --hello-world
  example --output-file=<filename>
  example -n <names>...

Options:
  -n <names>  bind -n with <names>
```

### Generate a CMake project

By setting the codegen mode to `cpp11_cmake_project` and passing the path of user defined synopsis, `clidoc` could generate a dedicated CMake project:

```
$ clidoc -m cpp11_cmake_project example_doc example_lib
```

Afterwards, move generated `example_lib` directory to an cmake project. Here we reuse the one defined in above section. Currently the project contains:

```
$ tree -L 2
.
├── example_doc
├── example_lib
│   ├── CMakeLists.txt
│   ├── include
│   └── src
└── main.cc

3 directories, 3 files
```

And create a `CMakeLists.txt` file:

```cmake
cmake_minimum_required (VERSION 2.8)
project (demo CXX)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -std=c++11")

add_subdirectory (example_lib)
include_directories (example_lib/include)

add_executable (example main.cc)
target_link_libraries (example clidoc_cpp11)
```

Finally, it's the time to build your project.


## Python Codegen

By setting the codegen mode to `python` and passing the path of user defined synopsis, `clidoc could generate a dedicated .py file:

```
$ clidoc -m python example_doc example.py
```

Generated `.py` file contains a function named `clidoc` and several flags:

```
__all__ = [
    'SYSTEM_EXIT_OFF',
    'PRINT_DOC_OFF',
    'GUIDELINE_8_OFF',
    'clidoc',
]


SYSTEM_EXIT_OFF = 1 << 0
PRINT_DOC_OFF = 1 << 1
GUIDELINE_8_OFF = 1 << 2


def clidoc(argv, flags=0):
    ...
```
where the function `clidoc` is able to process shell input values(i.e. `sys.argv`) with respect to user defined synopsis.

User could import such generated `.py` file and invoke function `clidoc` directly, without further configuration:

```python
➜  src python
Python 2.7.6 (default, Sep  9 2014, 15:04:36) 
[GCC 4.2.1 Compatible Apple LLVM 6.0 (clang-600.0.39)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>> from example import clidoc
>>> 
>>> argv = ['example', '--hello-world']
>>> clidoc(argv)
{u'--hello-world': True, u'-n': [], u'--output-file': u''}
>>>
>>> argv = ['example', '--output-file', 'filename']
>>> clidoc(argv)
{u'--hello-world': False, u'-n': [], u'--output-file': u'filename'}
>>>
>>> argv = ['example', '--output-file=filename']
>>> clidoc(argv)
{u'--hello-world': False, u'-n': [], u'--output-file': u'filename'}
>>>
>>> argv = ['example', '-n', 'a', 'b', 'c']
>>> clidoc(argv)
{u'--hello-world': False, u'-n': [u'a', u'b', u'c'], u'--output-file': u''}
>>>
>>> argv = ['example', '-n', 'a', '-n', 'b', '-n', 'c']
>>> clidoc(argv)
{u'--hello-world': False, u'-n': [u'a', u'b', u'c'], u'--output-file': u''}
>>>
>>> argv = ['example', '-n', 'a,b,c']
>>> clidoc(argv)
{u'--hello-world': False, u'-n': [u'a', u'b', u'c'], u'--output-file': u''}
```
