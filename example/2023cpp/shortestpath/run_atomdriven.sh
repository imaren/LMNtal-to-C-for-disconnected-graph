#!/bin/bash

# rm output2.csv
count=100


DIR='/home/imagawa/LMNtal-to-C-for-disconnected-graph/example/2023cpp/shortestpath'

g++ -o $DIR/shortestpath_atomdriven $DIR/shortestpath_atomdriven.cpp 


rm $DIR/result_shortestpath_atomdriven.csv

while true
do

    echo $count 

    sed -e 's/NUM/'$count'/g' $DIR/make_graph.cpp > $DIR/tmp.cpp

    g++ -o $DIR/tmp $DIR/tmp.cpp
    $DIR/tmp > $DIR/input.txt
    
    echo -n $count >> $DIR/result_shortestpath_atomdriven.csv
    echo -n , >> $DIR/result_shortestpath_atomdriven.csv

    $DIR/shortestpath_atomdriven

    if [ $? -eq 124 ]; then exit 0
    fi
    if [ $count -ge 800 ]; then exit 0
    fi
    count=`expr $count + 100`

done