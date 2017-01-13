# C Code Quality Metrics

[![Build Status](https://travis-ci.org/dspinellis/cqmetrics.svg?branch=master)](https://travis-ci.org/dspinellis/cqmetrics)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/8464/badge.svg)](https://scan.coverity.com/projects/dspinellis-cqmetrics)

The program qmcalc will read a C source code file from its standard
input and print on its standard output a single line containing
tens of metrics regarding size, complexity,
used keywords, comments and documentation, use of the C preprocessor,
and observed formatting style.
The full set of the metrics is documented in the file
[metrics.md](https://github.com/dspinellis/cqmetrics/blob/master/metrics.md).

The code has been written to run efficiently and reliably.
The program receives the code to analyze from its standard input,
so that it can process data coming e.g. directly out of a `git show` command,
without having intermediate data touch the disk.
Also, the program can process code
that may not be easy (or possible) to fully parse using a compiler front-end,
due e.g. to missing header files or an unknown compiler configuration.

Furthermore,
the software is designed in a way that should allow its easy extension to
other languages with C-like syntax, such as Java, C++, C#, and Go.
The main thing you need to add is an *X*MetricsCalculator and an
*X*Keyword class.
Pull requests for such extensions are more than welcomed.

## Building

### Prerequisites
* C++ compiler
* GNU make
* [CppUnit](http://sourceforge.net/projects/cppunit/) for running `make test`

To build type change the directory to `src` (`cd src`) and run `make`.
This will also generate the files `header.tab` and `header.txt`,
which can be used for loading the output into other programs
for further processing.

## Further reading
The style checks performed are based on the following guidelines.
* [Google](http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml)
* [FreeBSD](http://www.freebsd.org/cgi/man.cgi?query=style&sektion=9)
* [GNU](https://www.gnu.org/prep/standards/html_node/Formatting.html)
* [Recommended C Style and Coding Standards - updated version of the Indian Hill C Style and Coding Standards paper](http://www.cs.arizona.edu/~mccann/cstyle.html)
* [Wikipedia summary](http://en.wikipedia.org/wiki/Indent_style)

## Citation
* The tool is described in detail in the following paper.
Diomidis Spinellis, Panos Louridas, and Maria Kechagia. [The evolution of C programming practices: A study of the Unix operating system](http://www.dmst.aueb.gr/dds/pubs/conf/2016-ICSE-ProgEvol/html/SLK16.html) 1973–2015. In Willem Visser and Laurie Williams, editors, *ICSE '16: Proceedings of the 38th International Conference on Software Engineering*, pages 748–759, New York, May 2016. Association for Computing Machinery. ([doi:10.1145/2884781.2884799](http://dx.doi.org/10.1145/2884781.2884799))
* Some other metrics are based on the paper [A Tale of Four Kernels](http://www.dmst.aueb.gr/dds/pubs/conf/2008-ICSE-4kernel/html/Spi08b.html).
