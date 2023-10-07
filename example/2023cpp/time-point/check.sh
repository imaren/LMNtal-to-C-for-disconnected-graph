#!/bin/bash

max=550

for((i=550; i<= $max; i+=50)); do
    echo $i
    # for j in {1,2,3}; do
    j=2
        cat time-point.lmn > tmp.lmn
        cat testcase/L2_$i-$j >> tmp.lmn
        /usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 tmp.lmn > tmp.il
        slim -p tmp.il &> tmp_result
        if grep -q fail tmp_result; then
        echo fail_$i-$j
        fi
    # done
done