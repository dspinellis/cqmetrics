#!/bin/sh
#
# Summarize the formatting style metrics for the specified directory
#
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
