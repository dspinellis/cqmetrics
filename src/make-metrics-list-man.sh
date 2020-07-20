#!/bin/sh
#
# Generate the metrics.1 file
#
# The lists are automatically derived from the source code
#

cat <<\EOF
.\" Process this file with
.\" groff -man -Tascii cqmetrics.1
.\"
.TH Cmetrics 1 "July 2020" "User Manuals"
.SH NAME
qmcalc \- process its standard input or report metrics on each of the specified files
.SH SYNOPSIS
.B qmcalc [ file .. ]
.br
.B ./sumstyle.sh [ directory ... ]
.br
.B .showstyle.sh [ directory ... ]
.SH DESCRIPTION
The program
.I qmcalc
will read a C source code file from its standard input and
print on its standard output a single line containing tens
of metrics regarding size, complexity, used keywords, comments
and documentation, use of the C preprocessor, and observed formatting style.
The full set of the metrics is documented in the file metrics.md.
.PP
The
.BR showstyle
wrapper takes as a single argument a directory,
and reports the metrics for each C file in the directory
.PP
The
.BR sumstyle
wrapper takes as a single argument a directory,
processes all C files in the directory,
and for each metric from ncpp_directive and onward it reports:
.RS
.nf
- the metric's ordinal number,
- the sum of the metric's values over all files, and
- the percentage of files for which the metric is non-zero.
.RE

.PP
.BR
The list of metrics are calculated for each source file are:
EOF

# Generate list of primary metrics
sed -n 's/^\t\t\/\/ VAL: /.br\n\\(em /p' QualityMetrics.cpp
# Generate list of style metrics
sed -n '/^\tenum StyleHint {/,/Add more elements here/{
	/enum StyleHint/d
	/^[ \t]*\/\//d
	/Add more elements here/d
	s/./\l&/g
	s/\t\t//
	s/^/Number of occurences of /
	s/_/ /g
	s/^/.br\n\\(em /
	s/,$//
	s/ op$/ operator/
	p
}' QualityMetrics.h

cat <<\EOF
.SH BUGS
Please report any other bugs you find to the issue tracking system in Github repository
<https://github.com/dspinellis/cqmetrics> of the project.
More information about how to get in contact with developers and getting help
can be found there.
