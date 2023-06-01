#!/bin/bash

# rm output2.csv
count=100

g++ -o shortestpath_atomdriven shortestpath_atomdriven.cpp 

while true
do

    echo $count 

    sed -e 's/NUM/'$count'/g' ./make_graph.cpp > ./tmp.cpp

    g++ -o tmp tmp.cpp
    ./tmp > input.txt
    
    # echo -n $count >> output2.csv
    # echo -n , >> output2.csv

    ./shortestpath_atomdriven > check2.txt

    if [ $? -eq 124 ]; then exit 0
    fi
    if [ $count -ge 1 ]; then exit 0
    fi
    count=`expr $count + 100`

done