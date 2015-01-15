#!/bin/sh
#
# Print the formatting style metrics for the specified directory
#
find "$1" -type f -name \*.c |
while read f
do
	echo -n "$f "
	qmcalc <$f
done |
awk '{
	printf("%s ", $1)
	for (i = 38; i <= NF; i++) printf("%g ", $i)
		print ""
}'
