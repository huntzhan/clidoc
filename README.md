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

and apply `add_subdirectory (clidoc)` in `CMakeLists.txt` to include `clidoc`.

Thereafte, an example of `doc` is created:

```
# file `example_doc`.
Usage:
  example --hello-world
  example -n <names>...

Options:
  -n <names>  # bind -n with <names>
```

`example_doc` says that there is a program, named `example`, can be invoked by user with two kinds of input arguments. One with the form of `path/to/example --hello-world`, and the other with the form of `path/to/example -n one or more arguments`.




# Syntax of `doc`

coming soon.

# Interface of C++11

comming soon.

# Versions

comming soon.