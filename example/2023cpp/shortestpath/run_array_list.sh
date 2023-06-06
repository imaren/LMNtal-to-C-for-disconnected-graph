#!/bin/bash

# rm output2.csv
count=100


DIR='/home/imagawa/LMNtal-to-C-for-disconnected-graph/example/2023cpp/shortestpath'

# g++ -o $DIR/array $DIR/array.cpp 


rm $DIR/result_array_list.csv

while true
do

    echo $count 

    sed -e 's/NUM/'$count'/g' $DIR/make_graph.cpp > $DIR/tmp.cpp
    sed -e 's/VAR/'$count'/g' $DIR/array_list.cpp > $DIR/array_list_tmp.cpp

    g++ -o $DIR/array_list_tmp $DIR/array_list_tmp.cpp 

    g++ -o $DIR/tmp $DIR/tmp.cpp
    $DIR/tmp > $DIR/input.txt
    
    echo -n $count >> $DIR/result_array_list.csv
    echo -n , >> $DIR/result_array_list.csv

    $DIR/array_list_tmp

    if [ $? -eq 124 ]; then exit 0
    fi
    if [ $count -ge 800 ]; then exit 0
    fi
    count=`expr $count + 100`

done