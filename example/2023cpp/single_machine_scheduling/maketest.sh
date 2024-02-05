#!/bin/bash

max=12

for((i=1; i<= $max; i+=1)); do
    for j in {1,2,3} ; do
        ./makegraph $i > testcase/case_$i-$j
        tail -n 1 testcase/case_$i-$j > testcase/Lnd$i-$j
        tail -n 2 testcase/case_$i-$j | head -n 1 > testcase/L$i-$j
        head -n 1 testcase/case_$i-$j > testcase/C$i-$j
    done 
done