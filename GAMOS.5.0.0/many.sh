#!/bin/sh
#!

#rm lista.txt
find * |
while read filename ; do
  echo ${filename} 
  cvs add ${filename}
done

exit
