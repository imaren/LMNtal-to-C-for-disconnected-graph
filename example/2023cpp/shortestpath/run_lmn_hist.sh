#!/bin/bash

export LMNTAL_HOME=/home/imagawa/lmntal-compiler
rm result_hist.csv
count=10
while true
do

    echo $count 

    sed -e 's/NUM/'$count'/g' ./make_graph.cpp > ./tmp.cpp

    g++ -o tmp tmp.cpp
    ./tmp | ./script.sh > tmp.lmn

    /home/imagawa/lmntal-compiler/bin/lmntal --slimcode -O3  tmp.lmn > tmp.il
    
    timeout 2000 /home/imagawa/slim/src/slim -p2 --history-management tmp.il &> ./in.txt

    if [ $? -eq 124 ]; then exit 0
    fi
    ./format >> result_hist.csv  
    if [ $count -ge 100 ]; then exit 0
    fi
    count=`expr $count + 10`

done