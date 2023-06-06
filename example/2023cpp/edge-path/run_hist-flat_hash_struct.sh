#!/bin/bash

# rm output2.csv
count=500


DIR='/home/imagawa/LMNtal-to-C-for-disconnected-graph/example/2023cpp/edge-path'

g++ -o $DIR/hist-flat_hash_struct $DIR/hist-flat_hash_struct.cpp 


rm $DIR/result_hist-flat_hash_struct.csv

while true
do

    echo $count 

    sed -e 's/NUM/'$count'/g' $DIR/make_graph.cpp > $DIR/tmp.cpp

    g++ -o $DIR/tmp $DIR/tmp.cpp
    $DIR/tmp > $DIR/input.txt
    
    echo -n $count >> $DIR/result_hist-flat_hash_struct.csv
    echo -n , >> $DIR/result_hist-flat_hash_struct.csv

    $DIR/hist-flat_hash_struct

    if [ $? -eq 124 ]; then exit 0
    fi
    if [ $count -ge 10000 ]; then exit 0
    fi
    count=`expr $count + 500`

done