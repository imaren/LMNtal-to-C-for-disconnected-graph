#!/bin/bash

# rm output2.csv
count=50


DIR='/home/imagawa/LMNtal-to-C-for-disconnected-graph/example/2023cpp/shortestpath'

# g++ -o $DIR/array $DIR/array.cpp 

rm $DIR/result_array_vec_flathash_myhashfunc2.csv
rm $DIR/result_wf.csv

while true
do

    echo $count 

    sed -e 's/NUM/'$count'/g' $DIR/make_graph.cpp > $DIR/tmp.cpp
    sed -e 's/VAR/'$count'/g' $DIR/array_vec_flathash_myhashfunc2.cpp > $DIR/array_vec_flathash_myhashfunc2_tmp.cpp
    sed -e 's/VAR/'$count'/g' $DIR/wf.cpp > $DIR/wf_tmp.cpp

    g++ -o $DIR/array_vec_flathash_myhashfunc2_tmp $DIR/array_vec_flathash_myhashfunc2_tmp.cpp
    g++ -o $DIR/wf_tmp $DIR/wf_tmp.cpp

    # g++ -o $DIR/tmp $DIR/tmp.cpp
    # $DIR/tmp > $DIR/input.txt

    echo -n $count >> $DIR/result_array_vec_flathash_myhashfunc2.csv
    echo -n , >> $DIR/result_array_vec_flathash_myhashfunc2.csv
    echo -n $count >> $DIR/result_wf.csv
    echo -n , >> $DIR/result_wf.csv

    for ((i=0; i<1; i++)); do
        g++ -o $DIR/tmp $DIR/tmp.cpp
        $DIR/tmp > $DIR/input.txt
        $DIR/array_vec_flathash_myhashfunc2_tmp
        $DIR/wf_tmp
    done

        echo  , >> $DIR/result_array_vec_flathash_myhashfunc2.csv
        echo  , >> $DIR/result_wf.csv

    if [ $? -eq 124 ]; then exit 0
    fi
    if [ $count -ge 300 ]; then exit 0
    fi
    count=`expr $count + 50`

done