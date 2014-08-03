#!/bin/sh
#
# Produce a quality time series for all C files in the specified Git repository
#

cd "$1"
find . -name \*.c |
while read filename
do
        git log --pretty='format:%H %ae %ai' -- "$filename" |
        # ./dir/file.c 0506343883d62f6649f7bbaf1a436133cef6261d foo@example.com 1998-01-11 03:30:40 +0000
        while read hash email date hour zone
        do
                printf "$filename\t$hash\t$date\t$hour\t$zone\t"
                git show "$hash:$filename" | qmcalc
        done
done
