#!/bin/bash

max=150

for((i=25; i<= $max; i+=25)); do
    for j in {1,2,3}; do
    # j=2
        ../make_graph $i > case_4_$i-$j
        tail -n 1 case_4_$i-$j > L4_$i-$j
        sed -e '$d' case_4_$i-$j > C4_$i-$j
    done
done