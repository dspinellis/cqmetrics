# C Code Quality Metrics
The program qmcalc will read a C source code file from its standard
input and print on its standard output a single line with the following
numbers separated by a single tab.

* Number of characters
* Number of lines, minimum, mean, maximum, standard deviation of line length
* Number of functions (A function is considered to start with `{` at column 1)
* Number of statements, minimum, mean, maximum, standard deviation of statement nesting
* Number of declarations with internal linkage (`static`) visibility
* Number of `const` keywords
* Number of `enum` keywords
* Number of `goto` statements
* Number of `inline` keywords
* Number of `noalias` keywords
* Number of `register` keywords
* Number of `restrict` keywords
* Number of `signed` keywords
* Number of `struct` keywords
* Number of `union` keywords
* Number of `unsigned` keywords
* Number of `void` keywords
* Number of `volatile` keywords
* Number of `typedef` keywords
* Number of comments
* Number of comment characters
* Number of boilerplate (license) comment characters
* Number of DOxygen comments
* Number of DOxygen comment characters
* Number of comments inside function bodies
* Number of C preprocessor directives
* Number of C preprocessor include directives
* Number of C preprocessor conditional directives (`#if`, `#ifdef`, `#elif`)
* Number of C preprocessor directives in function bodies
* Number of C preprocessor conditional directives in function bodies
* Style inconsistency
* Number of functions, minimum, mean, maximum, standard deviation of Halstead complexity per function
* Number of functions, minimum, mean, maximum, standard deviation of cyclomatic complexity per function
* Number of identifiers, minimum, mean, maximum, standard deviation of identifier length
* Number of unique identifiers, minimum, mean, maximum, standard deviation of unique identifier length
* Number spaces used for one level of indentation, minimum, mean, maximum, standard deviation of spaces used for one level of indentation
* Number of occurences of no space after binary operator
* Number of occurences of no space after closing brace
* Number of occurences of no space after comma
* Number of occurences of no space after keyword
* Number of occurences of no space after opening brace
* Number of occurences of no space after semicolon
* Number of occurences of no space before binary operator
* Number of occurences of no space before closing brace
* Number of occurences of no space before keyword
* Number of occurences of no space before opening brace
* Number of occurences of space after opening square bracket
* Number of occurences of space after struct operator
* Number of occurences of space after unary operator
* Number of occurences of space at end of line
* Number of occurences of space before closing bracket
* Number of occurences of space before closing square bracket
* Number of occurences of space before comma
* Number of occurences of space before opening square bracket
* Number of occurences of space before semicolon
* Number of occurences of space before struct operator
* Number of occurences of space after binary operator
* Number of occurences of space after closing brace
* Number of occurences of space after comma
* Number of occurences of space after keyword
* Number of occurences of space after opening brace
* Number of occurences of space after semicolon
* Number of occurences of no space after struct operator
* Number of occurences of space before binary operator
* Number of occurences of space before closing brace
* Number of occurences of space before keyword
* Number of occurences of space before opening brace
* Number of occurences of no space before struct operator
* Number of occurences of no space after opening square bracket
* Number of occurences of no space after unary operator
* Number of occurences of no space before closing bracket
* Number of occurences of no space before closing square bracket
* Number of occurences of no space before comma
* Number of occurences of no space before opening square bracket
* Number of occurences of no space before semicolon


## Building
### Prerequisites
* C++ compiler
* GNU make
* [CppUnit](http://sourceforge.net/projects/cppunit/) for running `make test`

To build type `make`. This will also generate the file `header.tab`,
which can be used for loading the output into other programs
for further processing.

## Further reading
The style checks performed are based on the following guidelines.
* [Google](http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml)
* [FreeBSD](http://www.freebsd.org/cgi/man.cgi?query=style&sektion=9)
* [GNU](https://www.gnu.org/prep/standards/html_node/Formatting.html)
* [Recommended C Style and Coding Standards - updated version of the Indian Hill C Style and Coding Standards paper](http://www.cs.arizona.edu/~mccann/cstyle.html
* [Wikipedia summary](http://en.wikipedia.org/wiki/Indent_style)

Some other metrics are based on the paper [A Tale of Four Kernels](http://www.dmst.aueb.gr/dds/pubs/conf/2008-ICSE-4kernel/html/Spi08b.html)
