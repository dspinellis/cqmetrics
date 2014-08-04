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
* No space after binary operator
* No space after closing brace
* No space after comma
* No space after keyword
* No space after opening brace
* No space after semicolon
* No space after struct operator
* No space before binary operator
* No space before closing brace
* No space before keyword
* No space before opening brace
* No space before struct operator
* Space after opening square bracket
* Space after unary operator
* Space at end of line
* Space before closing bracket
* Space before closing square bracket
* Space before comma
* Space before opening square bracket
* Space before semicolon

The numbers are separated by a single tab.

## Building
### Prerequisites
* C++ compiler
* GNU make
* [CppUnit](http://sourceforge.net/projects/cppunit/)

* To build type `make`
