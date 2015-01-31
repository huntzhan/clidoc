# What is `clidoc`

Generally, `clidoc` is a source-to-source compiler, which is inspired by [`docopt`](https://github.com/docopt/docopt).
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

# Quick start

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

```
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

coming soon.

# Interface of C++11

comming soon.

# Versions

comming soon.