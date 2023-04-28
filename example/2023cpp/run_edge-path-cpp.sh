#!/bin/bash

rm output.csv
count=100

g++ -o edge-path_speicalized -O3 edge-path_specialized.cpp 

while true
do

    echo $count 

    sed -e 's/NUM/'$count'/g' ./make_graph.cpp > ./tmp.cpp

    g++ -o tmp tmp.cpp
    ./tmp > input.txt
    
    echo -n $count >> output.csv
    echo -n , >> output.csv

    ./edge-path_speicalized

    if [ $? -eq 124 ]; then exit 0
    fi
    if [ $count -ge 10000 ]; then exit 0
    fi
    count=`expr $count + 100`

done