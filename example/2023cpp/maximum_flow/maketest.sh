#!/bin/bash

max=1000
max2=10000

for((i=100; i<= $max; i+=100)); do
    for((j=1;j<=100;j++)); do
        ./makegraph $i > testcase/m2nu50/case_$i-$j
        tail -n 1 testcase/m2nu50/case_$i-$j | head -n 1 > testcase/m2nu50/L$i-$j
        head -n 1 testcase/m2nu50/case_$i-$j > testcase/m2nu50/C$i-$j
    done 
done

for((i=2000; i<= $max2; i+=1000)); do
    for((j=1;j<=100;j++)); do
        ./makegraph $i > testcase/m2nu50/case_$i-$j
        tail -n 1 testcase/m2nu50/case_$i-$j | head -n 1 > testcase/m2nu50/L$i-$j
        head -n 1 testcase/m2nu50/case_$i-$j > testcase/m2nu50/C$i-$j
    done 
done