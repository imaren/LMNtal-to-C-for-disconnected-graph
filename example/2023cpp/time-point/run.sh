#!/bin/bash

rm result_lmn.csv
# rm result_hlopt.csv
# echo -n "" > result_flathash_myhashfunc.csv
# echo -n "" > result_flathash_myhashfuncO3.csv


max=100

for((i=100; i<= $max; i+=25)); do
    echo $i
    for j in {2,3}; do
        cat time-point.lmn > tmp.lmn
        cat testcase/L4_$i-$j >> tmp.lmn
        /usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 tmp.lmn > tmp.il
        slim -p tmp.il &> tmp_result
        grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result >> result_lmn.csv
        # cat time-point.lmn > tmp.lmn
        # cat testcase/L$i-$j >> tmp.lmn
        # /usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 --hl-opt tmp.lmn > tmp.il
        # slim -p tmp.il &> tmp_result
        # grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result >> result_hlopt.csv
        # ./flathash_myhashfunc testcase/C4_$i-$j
        # echo "" >> result_flathash_myhashfunc.csv
        # ./flathash_myhashfuncO3 testcase/C4_$i-$j
        # echo "" >> result_flathash_myhashfuncO3.csv
    done
done