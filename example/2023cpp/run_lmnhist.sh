#!/bin/bash

export LMNTAL_HOME=/home/imagawa/lmntal-compiler
# rm result_hist.csv
count=100
while true
do

    echo $count 

    sed -e 's/NUM/'$count'/g' ./make_graph.cpp > ./tmp.cpp

    g++ -o tmp tmp.cpp
    ./tmp | ./script.sh > tmp.lmn

    /home/imagawa/lmntal-compiler/bin/lmntal --slimcode -O3  tmp.lmn > tmp.il
    
    /home/imagawa/slim/src/slim --history-management tmp.il > check1.txt
    # /home/imagawa/slim/src/slim  tmp.il > check1.txt

    if [ $? -eq 124 ]; then exit 0
    fi
    # ./format >> result_hist.csv  
    if [ $count -ge 1 ]; then exit 0
    fi
    count=`expr $count + 100`

done