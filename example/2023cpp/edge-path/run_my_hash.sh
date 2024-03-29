#!/bin/bash

# rm output2.csv
count=500


DIR='/home/imagawa/LMNtal-to-C-for-disconnected-graph/example/2023cpp/edge-path'

g++ -o $DIR/hist-my_hash $DIR/hist-my_hash.cpp 


rm $DIR/result_hist-my_hash.csv

while true
do

    echo $count 

    sed -e 's/NUM/'$count'/g' $DIR/make_graph.cpp > $DIR/tmp.cpp

    g++ -o $DIR/tmp $DIR/tmp.cpp
    $DIR/tmp > $DIR/input.txt
    
    echo -n $count >> $DIR/result_hist-my_hash.csv
    echo -n , >> $DIR/result_hist-my_hash.csv

    $DIR/hist-my_hash

    if [ $? -eq 124 ]; then exit 0
    fi
    if [ $count -ge 10000 ]; then exit 0
    fi
    count=`expr $count + 1000`

done