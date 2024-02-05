#!/bin/bash

# echo -n "" > result_lmn.csv
# rm result_hlopt.csv
# echo -n "" > result_flathash_myhashfunc.csv
# echo -n "" > result_flathash_myhashfuncO3.csv
# echo -n "" > result_delfirst.csv
echo -n "" > result_delfirst_front.csv
# echo -n "" > result_memreduce.csv

max=1000
dir=testcase/m2nu50

# for((i=500; i<= $max; i+=100)); do
#     echo $i
#     for((j=1;j<=100;j++)); do
#         cat maxflow_fixed.lmn > tmp.lmn
#         cat $dir/L$i-$j >> tmp.lmn
#         /usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 tmp.lmn > tmp.il
#         /usr/bin/time -f "%M" -o tmp.txt slim -p --no-dump tmp.il &> tmp_result
#         grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result | tr "\n" "," >> result_lmn.csv
#         cat tmp.txt >> result_lmn.csv
#         # cat time-point.lmn > tmp.lmn
#         # cat testcase/L$i-$j >> tmp.lmn
#         # /usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 --hl-opt tmp.lmn > tmp.il
#         # slim -p tmp.il &> tmp_result
#         # grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result >> result_hlopt.csv
#         # ./flathash_myhashfunc testcase/C4_$i-$j
#         # echo "" >> result_flathash_myhashfunc.csv
#         # ./flathash_myhashfuncO3 testcase/C2n_$i-$j
#         # echo "" >> result_flathash_myhashfuncO3.csv
#         /usr/bin/time -f "%M" -o tmp.txt ./delfirst_opt $dir/C$i-$j
#         cat tmp.txt >> result_delfirst.csv
#         /usr/bin/time -f "%M" -o tmp.txt ./delfirst_fromfront $dir/C$i-$j
#         cat tmp.txt >> result_delfirst_front.csv
#     done
# done

max2=10000
for((i=1000; i<= $max2; i+=1000)); do
    echo $i
    for((j=1;j<=100;j++)); do
        # /usr/bin/time -f "%M" -o tmp.txt ./delfirst_opt $dir/C$i-$j
        # cat tmp.txt >> result_delfirst.csv
        /usr/bin/time -f "%M" -o tmp.txt ./delfirst_front $dir/C$i-$j
        cat tmp.txt >> result_delfirst_front.csv
    done
done