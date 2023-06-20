#!/bin/bash

# rm output2.csv
count=500

g++ -o edge-path_atomdriven_myhashfunc edge-path_atomdriven_myhashfunc.cpp 

DIR='/home/imagawa/LMNtal-to-C-for-disconnected-graph/example/2023cpp/edge-path'

rm $DIR/result_stl_myhashfunc.csv

while true
do

    echo $count 

    sed -e 's/NUM/'$count'/g' $DIR/make_graph.cpp > $DIR/tmp.cpp

    g++ -o $DIR/tmp $DIR/tmp.cpp
    $DIR/tmp > $DIR/input.txt
    
    echo -n $count >> $DIR/result_stl_myhashfunc.csv
    echo -n , >> $DIR/result_stl_myhashfunc.csv

    $DIR/edge-path_atomdriven_myhashfunc 

    if [ $? -eq 124 ]; then exit 0
    fi
    if [ $count -ge 10000 ]; then exit 0
    fi
    count=`expr $count + 500`

done