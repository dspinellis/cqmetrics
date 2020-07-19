#!/bin/sh
#
# Summarize the formatting style metrics for the specified directory
#
if  ! qmcalc </dev/null >/dev/null 2>&1; then
  echo 'Error: qmcalc is not installed.' >&2
  exit 1
fi
if [ "$1" = "-h" ] || [ $# -eq 0 ]; then
  echo "Usage: `basename $0` [-h] <target directory> -- processes all C files in the directory
	 and for each metric from ncpp_directive and onward reports:
	 - the metric's ordinal number,
	 - the sum of the metric's values over all files, and
	 - the percentage of files for which the metric is non-zero."
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
awk '
{
	count++;
	for (i = 38; i <= (nf = NF); i++) {
		s[i] += $i
		if ($i)
			o[i]++
	}
}
END {
	for (i = 38; i <= nf; i++)
		print i, s[i], o[i] / count * 100
}' |
sort -k 3nr
