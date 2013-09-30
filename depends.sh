#!/bin/sh
# this script is written based on the [Recursive Make Considered Harmful](http://aegis.sourceforge.net/auug97.pdf) paper.
DIR=$(dirname $1)
shift 1
case "$DIR" in
	"" | ".")
		gcc -MM -MG "$@" | sed -e "s@^\(.*\)\.o:@\1.d \1.o:@"
		;;
	*)
		gcc -MM -MG "$@" | sed -e "s@^\(.*\)\.o:@$DIR/\1.d $DIR/\1.o:@"
		;;
esac
