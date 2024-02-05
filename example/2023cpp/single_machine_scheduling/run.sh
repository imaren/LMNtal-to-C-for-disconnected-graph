#!/bin/bash

echo "" > result_lmns.csv
echo "" > result_lmnc.csv
echo "" > result_lmnn.csv

max=10
for((i=1; i<= $max; i+=1)); do
    echo $i
    for j in {1,2,3} ; do
        echo " $j"
        cat single_machine_scheduling_simple.lmn > tmps.lmn
        cat testcase/L$i-$j >> tmps.lmn
        cat single_machine_scheduling_simple_onlycost.lmn > tmpc.lmn
        cat testcase/L$i-$j >> tmpc.lmn
        cat single_machine_scheduling_simple_nd.lmn > tmpn.lmn
        cat testcase/L$i-$j >> tmpn.lmn
        
        slim -p tmps.lmn &> tmp_result
        grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result >> result_lmns.csv
        slim -p tmpc.lmn &> tmp_result
        grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result >> result_lmnc.csv
        slim -p --nd tmpn.lmn &> tmp_result
        grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result >> result_lmnn.csv

    done 
done