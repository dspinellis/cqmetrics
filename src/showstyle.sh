#!/bin/sh
#
# Print the formatting style metrics for the specified directory
#
if ! qmcalc </dev/null >/dev/null 2>&1; then
  echo 'Error: qmcalc is not installed.' >&2
  exit 1
fi
if [ "$1" = "-h" ] || [ $# -eq 0 ]; then
  echo "Usage: `basename $0` [-h] <target directory> -- reports the metrics for each C file in the directory"
  exit 0
fi
if [ ! -d "$1" ]; then
	echo 'Error: '$1' directory does not exist' >&2
  exit 1
fi
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
