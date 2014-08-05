#!/bin/sh
#
# Generate the README.md file
#
# The lists are automatically derived from the source code
#

{
	# Generate list of primary metrics
	sed -n '
	# Skip formatting style metrics
	/q\.get_style_error/d
	# Print metrics variable
	s/.*q\.get_\([^(]*\)().*/\1/p' QualityMetrics.cpp |
	# Add further elements for descriptive metrics
	while read metric
	do
		if grep -q "> $metric;" QualityMetrics.h
		then
			echo "${metric}_count ${metric}_min ${metric}_mean ${metric}_max"
		else
			echo $metric
		fi
	done |
	# Convert _count metrics to the value measured
	sed '
	s/line_length_count/nline/
	s/statement_nesting_count/nstatement/
	s/halstead_count/nfunction/
	s/cyclomatic_count/nfunction2/
	s/identifier_length_count/nidentifier/
	s/unique_identifier_length_count/nunique_identifier/
	'

	# Generate list of style metrics
	sed -n '/^\tenum StyleError {/,/Add more elements here/{
		/enum StyleError/d
		/Add more elements here/d
		s/,$//
		s/\t\t/N/
		s/./\l&/g
		p
	}' QualityMetrics.h

} |
# Make the file tab-separated
tr ' \n' '\t\t' |
# Make trailing tab a newline
sed 's/\t$/\n/'
