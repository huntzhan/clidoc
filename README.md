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

# Documentation

* [Quick Start](https://github.com/haoxun/clidoc/wiki/Quick-Start)
* [Syntax of doc](https://github.com/haoxun/clidoc/wiki/Syntax-of-%60doc%60)
	* [Preprocessing Strategy](https://github.com/haoxun/clidoc/wiki/Preprocessing-Strategy)
	* [Lexical Rules](https://github.com/haoxun/clidoc/wiki/Lexical-Rules)
	* [Context-free Grammer of doc](https://github.com/haoxun/clidoc/wiki/Context-free-Grammer-of-%60doc%60)
* [Post Processing of AST](https://github.com/haoxun/clidoc/wiki/Post-Processing-of-AST)
* [Input Argument Processing](https://github.com/haoxun/clidoc/wiki/Input-Argument-Processing)
* Language Specific Interface:
	* [Interface of Cpp11](https://github.com/haoxun/clidoc/wiki/Interface-of-Cpp11)

# Change log

* `v0.1`: demo of `clidoc`.
* `v0.1.1`: bug fixed version.


[docopt]: https://github.com/docopt/docopt "docopt creates beautiful command-line interfaces."
[POSIX.1-2008]: http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap12.html "IEEE Std 1003.1, 2013 Edition: 12. Utility Conventions"
[AST]: http://en.wikipedia.org/wiki/Abstract_syntax_tree "Abstract syntax tree - Wikipedia, the free encyclopedia"

