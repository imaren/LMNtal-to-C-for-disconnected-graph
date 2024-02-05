#!/bin/bash

# rm result_lmn.csv
rm result_hist.csv
# rm result_hlopt.csv
# echo -n "" > result_flathash_myhashfunc.csv
# echo -n "" > result_flathash_myhashfuncO3.csv
# echo -n "" > result_delfirst.csv
# echo -n "" > result_delfirst_fromfront.csv
# echo -n "" > result_memreduce.csv

max=5000

for((i=100; i<= 600; i+=100)); do
    echo $i
    # for j in {1,2,3}; do
    j=1
        # cat time-point.lmn > tmp.lmn
        # cat testcase/L2n_$i-$j >> tmp.lmn
        # /usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 tmp.lmn > tmp.il
        # /usr/bin/time -f "%M" -o tmp.txt slim -p tmp.il &> tmp_result
        # grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result | tr "\n" "," >> result_lmn.csv
        # cat tmp.txt >> result_lmn.csv
        cat time-point_int.lmn > tmp.lmn
        cat testcase/I2n_$i-$j >> tmp.lmn
        /usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 tmp.lmn > tmp.il
        /usr/bin/time -f "%M" -o tmp.txt slim -p --history-management tmp.il &> tmp_result
        grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result | tr "\n" "," >> result_hist.csv
        cat tmp.txt >> result_hist.csv
        # cat time-point.lmn > tmp.lmn
        # cat testcase/L$i-$j >> tmp.lmn
        # /usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 --hl-opt tmp.lmn > tmp.il
        # slim -p tmp.il &> tmp_result
        # grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result >> result_hlopt.csv
        # ./flathash_myhashfunc testcase/C4_$i-$j
        # echo "" >> result_flathash_myhashfunc.csv
        # ./flathash_myhashfuncO3 testcase/C2n_$i-$j
        # echo "" >> result_flathash_myhashfuncO3.csv
        # ./delfirst testcase/C2n_$i-$j
        # echo "" >> result_delfirst.csv
        # /usr/bin/time -f "%M" -o tmp.txt ./delfirst_algo testcase/C2n_$i-$j
        # cat tmp.txt >> result_delfirst.csv
        # /usr/bin/time -f "%M" -o tmp.txt ./delfirst_fromfront testcase/C2n_$i-$j
        # cat tmp.txt >> result_delfirst_fromfront.csv
        # ./delfirst_fromfront testcase/C2n_$i-$j
        # echo "" >> result_delfirst.csv
        # ./memreduce testcase/C2n_$i-$j
        # echo "" >> result_memreduce.csv
    # done
done


for((i=6000; i<= 11000; i+=1000)); do
    echo $i
    j=1
        # /usr/bin/time -f "%M" -o tmp.txt ./delfirst_algo testcase/C2n_$i-$j
        # cat tmp.txt >> result_delfirst.csv
        # /usr/bin/time -f "%M" -o tmp.txt ./delfirst_fromfront testcase/C2n_$i-$j
        # cat tmp.txt >> result_delfirst_fromfront.csv
        # ./delfirst_fromfront testcase/C2n_$i-$j
        # echo "" >> result_delfirst.csv
done