contents="$1/contents"
changes="$1/changes"
if [ ! -d "$contents" ]; then
  echo "Invalid target.";
  exit -1;
fi
echo "Uploading... [target=$contents; changes=$changes]"
rm -rf "$changes"
mkdir "$changes"
find *.sh src test "$contents" \! -name \*~\* -and \! -name \*.out -and \! -name \*.o -and -type f -printf "%p %TY-%Tm-%Td %TH:%TM\n" | replace "$contents/" "" | sort | uniq -u | cut -d\  -f1 | uniq | while read ff; do
  install -D -p -v "$ff" "$contents/$ff"
  install -D -p -v "$ff" "$changes/$ff"
done
echo "Done"
