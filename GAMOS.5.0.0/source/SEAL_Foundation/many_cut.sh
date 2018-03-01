#!/bin/sh
#!

ls *.cpp |
while read filename ; do
  dirname="`echo "$filename" | cut -d "." -f1`"
  echo "File: $dirname"
mv ${dirname}.cpp ${dirname}.cc
done
exit
