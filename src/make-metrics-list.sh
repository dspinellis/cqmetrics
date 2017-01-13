#!/bin/sh
#
# Generate the metrics.md file
#
# The lists are automatically derived from the source code
#

cat <<\EOF
# C Code Quality Metrics
The program qmcalc will read a C source code file from its standard
input and print on its standard output a single line with the following
numbers separated by a single tab.
After building, the file `header.tab`
contains a tab-separated list of the metric names.
You can prepend this to the generated numbers so that you can read them
into named columns for further processing using e.g. R's `read.csv`
or Python's `csv.reader`.
Furthermore, the file `header.txt`
contains the ordinal number of each field (starting from 1),
which you can use to quickly access a particular field through its index,
e.g. from awk.

EOF

# Generate list of primary metrics
sed -n 's/^\t\t\/\/ VAL: /* /p' QualityMetrics.cpp
# Generate list of style metrics
sed -n '/^\tenum StyleHint {/,/Add more elements here/{
	/enum StyleHint/d
	/^[ \t]*\/\//d
	/Add more elements here/d
	s/./\l&/g
	s/\t\t//
	s/^/Number of occurences of /
	s/_/ /g
	s/^/* /
	s/,$//
	s/ op$/ operator/
	p
}' QualityMetrics.h
