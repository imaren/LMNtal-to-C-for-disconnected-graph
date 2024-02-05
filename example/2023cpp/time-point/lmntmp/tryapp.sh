#!/bin/bash
num=50
max=600
# ver=1
for((i=50; i<= $max; i+=50)); do
for ver in {1,2,3}; do

grep 'fprop' p2_L2_$i-$ver > tmp.txt
grep -r -o '[0-9]\+' tmp.txt > tmp2.txt
sed -n '2,3p' tmp2.txt | tr '\n' ',' | sed 's/,$//' >> forward.csv
echo "" >> forward.csv
grep 'bprop' p2_L2_$i-$ver > tmp.txt
grep -r -o '[0-9]\+' tmp.txt > tmp2.txt
sed -n '2,3p' tmp2.txt | tr '\n' ',' | sed 's/,$//' >> backward.csv
echo "" >> backward.csv
done
done