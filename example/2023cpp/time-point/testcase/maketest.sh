#!/bin/bash

max=5000

for((i=500; i<= $max; i+=500)); do
    # for j in {1,2,3}; do
    j=1
        ../make_graph $i > case_2n_$i-$j
        tail -n 1 case_2n_$i-$j > L2n_$i-$j
        sed -e '$d' case_2n_$i-$j > C2n_$i-$j
    # done
done