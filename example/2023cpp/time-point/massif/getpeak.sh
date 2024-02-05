#!/bin/bash

num=50
max=600
# ver=1
for((i=50; i<= $max; i+=50)); do
for ver in {1,2,3}; do
# grep -B 3  =peak massif.L2_$i-$ver > ./peak/massif.L2_$i-$ver
# grep -B 3  =peak massif.C2_$i-$ver > ./peak/memreduce.C2_$i-$ver
# grep -B 3  =peak set_C2_$i-$ver > ./peak/set_C2_$i-$ver
grep -B 3  =peak delfirst_C2_$i-$ver > ./peak/delfirst_C2_$i-$ver
done
done