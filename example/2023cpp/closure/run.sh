#!/bin/bash

rm result_lmn.csv
# rm result_history.csv
# echo -n "" > result_cpp.csv

max=100
max2=1000

for((i=10; i<= 80; i+=10)); do
    echo $i
    j=1
        cat path.lmn > tmp.lmn
        cat testcase/L$i >> tmp.lmn
        /usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 tmp.lmn > tmp.il
        /usr/bin/time -f "%M" -o tmp.txt slim -p tmp.il &> tmp_result
        grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result | tr "\n" "," >> result_lmn.csv
        cat tmp.txt >> result_lmn.csv
        
        # cat pathi.lmn > tmp.lmn
        # cat testcase/I$i >> tmp.lmn
        # /usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 tmp.lmn > tmp.il
        # /usr/bin/time -f "%M" -o tmp.txt slim -p --history-management tmp.il &> tmp_result
        # grep -E -o -m 1 '[0-9]+[.][0-9]+' tmp_result | tr "\n" "," >> result_history.csv
        # cat tmp.txt >> result_history.csv

        # /usr/bin/time -f "%M" -o tmp.txt ./delfirst_fromfront testcase/C$i
        # cat tmp.txt >> result_cpp.csv
done


for((i=200; i<= max2; i+=100)); do
    echo $i
    j=1
        # cat pathi.lmn > tmp.lmna

#         /usr/bin/time -f "%M" -o tmp.txt ./delfirst_fromfront testcase/C$i
#         cat tmp.txt >> result_cpp.csv
done