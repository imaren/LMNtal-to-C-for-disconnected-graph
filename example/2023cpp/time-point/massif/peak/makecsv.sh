#!/bin/bash

echo , > delfirst_C2.csv

num=50
max=600
# ver=1
for((i=50; i<= $max; i+=50)); do
for ver in {1,2,3}; do
# grep -m 1 -o '[0-9]\+' massif.L2_$i-$ver >> L2.csv
# grep -m 1 -o '[0-9]\+' memreduce.C2_$i-$ver >> memreduce_C2.csv
# grep -m 1 -o '[0-9]\+' set_C2_$i-$ver >> set_C2.csv
grep -m 1 -o '[0-9]\+' delfirst_C2_$i-$ver >> delfirst_C2.csv
done
done