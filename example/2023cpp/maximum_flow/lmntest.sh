#!/bin/bash

# echo -n "" > result_checkans

dir="diff"
file="diff/Lt"

n=100
        ./makegraph $n > $dir/test
        head -n 1 $dir/test > $dir/Ct
        tail -n 1 $dir/test > $dir/Lt

        cat maxflow_fixed_showmax.lmn > tmp.lmn
        cat $dir/Lt >> tmp.lmn
        /usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 tmp.lmn > tmp.il
        slim --shuffle-atom tmp.il > check1.txt
        ./tmp $dir/Ct > check2.txt
        ./checkans