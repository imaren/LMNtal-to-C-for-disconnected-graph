#!/bin/bash

echo -n "" > result_checkans

max=200

for((i=100; i<= $max; i+=100)); do
    echo $i
    for j in {1,2,3}; do
    # j=1   
        cat maxflow_fixed_showmax.lmn > tmp.lmn
        cat testcase/L$i-$j >> tmp.lmn
        /usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 tmp.lmn > tmp.il
        slim tmp.il > check1.txt
        # ./mem_print testcase/C$i-$j > check2.txt
        ./delfirst_fromfront testcase/C$i-$j > check2.txt
        ./checkans >> result_checkans
    done
done