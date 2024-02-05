#!/bin/bash

# echo -n "" > result_lmnc.csv
# echo -n "" > result_lmnnd.csv
# echo -n "" > result_lmnm.csv

max=10
for((i=7; i<= $max; i+=1)); do
    echo $i
    for j in {1,2,3} ; do
        echo " $j"
        # cat nd.lmn > tmpnd.lmn
        # cat testcase/L$i-$j >> tmpnd.lmn
        # cat permutation.lmn > tmpc.lmn
        # cat testcase/L$i-$j >> tmpc.lmn
        cat membrane.lmn > tmpm.lmn
        cat testcase/L$i-$j >> tmpm.lmn


        # slim -p --nd tmpnd.lmn &> tmp_result
        # grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result >> result_lmnnd.csv
        # slim -p tmpc.lmn &> tmp_result
        # grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result >> result_lmnc.csv
        slim -p tmpm.lmn &> tmp_result
        grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result >> result_lmnm.csv
    done 
done