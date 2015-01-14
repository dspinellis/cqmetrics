#!/usr/bin/sed -nf
/^\tenum StyleHint {/,/Add more elements here/{
	# Remove first line
	/enum StyleHint/d
	# Remove comments
	/^[ \t]*\/\//d
	s/\t\t/"/
	s/,$/",/
	p
}
