#!/bin/sh
 for i1 in 0 1 2 3 4 5 6 7 8 9; do
 for i2 in 0 1 2 3 4 5 6 7 8 9; do
 for i3 in 0 1 2 3 4 5 6 7 8 9; do
 for i4 in 0 1 2 3 4 5 6 7 8 9; do
#echo $i1$i2$i3$i4
#mv initialSeeds.1e10.$i1$i2$i3$i4 initialSeeds.1e+10.$i1$i2$i3$i4
#cvs delete initialSeeds.1e10.$i1$i2$i3$i4
#cvs add initialSeeds.1e+10.$i1$i2$i3$i4
cvs commit -m " " initialSeeds.1e+10.$i1$i2$i3$i4
done
done
done
done

cvsco
