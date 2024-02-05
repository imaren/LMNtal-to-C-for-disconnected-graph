#!/bin/bash

max=5000
max2=1000

for((i=100; i<= 600; i+=100)); do
    echo $i
    j=1
    ./a_c2li < C2_$i-1 > I2n_$i-1
done

