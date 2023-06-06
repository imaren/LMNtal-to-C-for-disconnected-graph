#!/bin/bash

# rm output2.csv
count=500


DIR='/home/imagawa/LMNtal-to-C-for-disconnected-graph/example/2023cpp/edge-path'

g++ -o $DIR/hist-flat_hash_fmt $DIR/hist-flat_hash_fmt.cpp 


rm $DIR/result_hist-flat_hash_fmt.csv

while true
do

    echo $count 

    sed -e 's/NUM/'$count'/g' $DIR/make_graph.cpp > $DIR/tmp.cpp

    g++ -o $DIR/tmp $DIR/tmp.cpp
    $DIR/tmp > $DIR/input.txt
    
    echo -n $count >> $DIR/result_hist-flat_hash_fmt.csv
    echo -n , >> $DIR/result_hist-flat_hash_fmt.csv

    $DIR/hist-flat_hash_fmt

    if [ $? -eq 124 ]; then exit 0
    fi
    if [ $count -ge 10000 ]; then exit 0
    fi
    count=`expr $count + 500`

done