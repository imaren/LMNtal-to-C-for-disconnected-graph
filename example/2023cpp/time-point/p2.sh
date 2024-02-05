#!/bin/bash
num=50
max=600
# ver=1
for((i=50; i<= $max; i+=50)); do
for ver in {1,2,3}; do
cat time-point.lmn > tmp.lmn
cat testcase/L2_$i-$ver >> tmp.lmn
/usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 tmp.lmn > tmp.il

/home/imagawa/tmp/slim/src/slim -p2 tmp.il &> ./lmntmp/tmp.txt 
grep -r 'propa' ./lmntmp/tmp.txt > ./lmntmp/p2_L2_$i-$ver

done
done