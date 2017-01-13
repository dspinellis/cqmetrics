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
