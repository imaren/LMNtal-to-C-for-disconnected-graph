#!/bin/bash

max=550

for((i=550; i<= $max; i+=50)); do
    # for j in {1,2,3}; do
    j=2
        ../make_graph $i > case_2_$i-$j
        tail -n 1 case_2_$i-$j > L2_$i-$j
        sed -e '$d' case_2_$i-$j > C2_$i-$j
    # done
done