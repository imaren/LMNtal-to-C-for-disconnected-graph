#!/bin/bash

rm result_lmn.csv
# rm result_hlopt.csv
# echo -n "" > result_flathash_myhashfunc.csv
# echo -n "" > result_flathash_myhashfuncO3.csv


max=600

for((i=50; i<= $max; i+=50)); do
    echo $i
    for j in {1,2,3}; do
        cat time-point.lmn > tmp.lmn
        cat testcase/L2_$i-$j >> tmp.lmn
        /usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 tmp.lmn > tmp.il
        slim -p tmp.il &> tmp_result
        grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result >> result_lmn.csv
        # cat time-point.lmn > tmp.lmn
        # cat testcase/L$i-$j >> tmp.lmn
        # /usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 --hl-opt tmp.lmn > tmp.il
        # slim -p tmp.il &> tmp_result
        # grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result >> result_hlopt.csv
        # ./flathash_myhashfunc testcase/C2_$i-$j
        # echo "" >> result_flathash_myhashfunc.csv
        # ./flathash_myhashfuncO3 testcase/C2_$i-$j
        # echo "" >> result_flathash_myhashfuncO3.csv
    done
done