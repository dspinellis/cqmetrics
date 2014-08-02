# C Code Quality Metrics
The program qmcalc will read a C source code file from its standard
input and print on its standard output a single line with the following
numbers.

* Number of characters
* Number of lines, minimum, mean, maximum line length
* Number of functions (A function is considered to start with `{` at column 1)
* Number of statements, minimum, mean, maximum statement nesting
* Number of `goto` statements declarations
* Number of `typedef`
* Number of comments
* Number of comment characters
* Number of comments inside function bodies
* Number of C preprocessor directives
* Number of C preprocessor include directives
* Number of C preprocessor conditional directives (`#if`, `#ifdef`, `#elif`)
* Number of C preprocessor directives in function bodies
* Number of C preprocessor conditional directives in function bodies
* Number of functions, minimum, mean, maximum Halstead complexity per function
* Number of functions, minimum, mean, maximum cyclomatic complexity per function
* Number of identifiers, minimum, mean, maximum identifier length
* Number of unique identifiers, minimum, mean, maximum unique identifier length

The numbers are separated by a single tab.

## Building
### Prerequisites
* C++ compiler
* GNU make
* [CppUnit](http://sourceforge.net/projects/cppunit/)

* To build type `make`
