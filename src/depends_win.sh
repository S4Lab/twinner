#!/bin/bash
DEP="$1"
DIR=$(dirname "$DEP")
shift 1

inc=$(cmd /C "C:\VC\vcvarsall.bat && cl /nologo /Zs /showIncludes /c $@")
case "$DIR" in
  "" | ".")
		echo "$DEP ${DEP/.d/.obj}: \\"
		echo "$inc" | grep '^Note: including file:' | gawk -F'Note: including file:' '{ print $2 }' | sed 's/^ */\t/g' | sed 's/$/ \\/'
    ;;
  *)
		echo "$DIR/$DEP $DIR/${DEP/.d/.obj}: \\"
		echo "$inc" | grep '^Note: including file:' | gawk -F'Note: including file:' '{ print $2 }' | sed 's/^ */\t/g' | sed 's/$/ \\/'
    ;;
esac
echo

