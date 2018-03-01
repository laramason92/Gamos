#!/bin/bash

rm rungm 
ls *dylib | while read file ; do
 file2=`echo $file | sed s/dylib/rootmap/g | sed s/lib//g `
# echo $file XXX $file2
echo "$ROOTSYS/bin/genmap --input-libray $file --output-file $file2 " >> rungm 
done
echo source rungm
