#!/bin/bash

max=100
max2=1000

for((i=10; i<= $max; i+=10)); do
    echo $i
    j=1
    ./c2li < C$i > I$i
done


for((i=200; i<= max2; i+=100)); do
    echo $i
    j=1
        ./c2li < C$i > I$i
done