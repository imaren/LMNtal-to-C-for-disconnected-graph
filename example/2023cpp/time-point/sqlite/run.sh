#!/bin/bash

rm result_timepoint_norec.csv


max=600

for((i=50; i<= $max; i+=50)); do
    echo $i
    for j in {1,2,3}; do
        ./execute2 ../testcase/C2_$i-$j
        echo "" >> result_timepoint_norec.csv
    done
done