#!/bin/bash

# rm output2.csv
count=5000


DIR='/home/imagawa/LMNtal-to-C-for-disconnected-graph/example/2023cpp/edge-path'
g++ -o edge-path_atomdriven edge-path_atomdriven.cpp 
g++ -o $DIR/hist-my_hash $DIR/hist-my_hash.cpp 
g++ -o $DIR/hist-flat_hash_fmt $DIR/hist-flat_hash_fmt.cpp 
g++ -o $DIR/hist-ideal_atomdriven $DIR/hist-ideal_atomdriven.cpp 
g++ -o $DIR/hist-flat_hash $DIR/hist-flat_hash.cpp 
g++ -o edge-path_atomdriven_myhashfunc edge-path_atomdriven_myhashfunc.cpp
g++ -o edge-path_atomdriven_myhashfunc2 edge-path_atomdriven_myhashfunc2.cpp
g++ -o $DIR/hist-flat_hash_myhashfunc $DIR/hist-flat_hash_myhashfunc.cpp
g++ -o $DIR/hist-flat_hash_myhashfunc2 $DIR/hist-flat_hash_myhashfunc2.cpp  

rm $DIR/result_hist-flat_hash.csv
rm $DIR/result_hist-flat_hash_myhashfunc.csv
rm $DIR/result_hist-flat_hash_myhashfunc2.csv
rm $DIR/result_hist-ideal.csv
rm $DIR/result_hist-my_hash.csv
rm $DIR/result_stl.csv
rm $DIR/result_stl_myhashfunc.csv
rm $DIR/result_stl_myhashfunc2.csv
rm $DIR/result_hist-flat_hash_fmt.csv

while true
do

    echo $count 

    sed -e 's/NUM/'$count'/g' $DIR/make_graph.cpp > $DIR/tmp.cpp

    # g++ -o $DIR/tmp $DIR/tmp.cpp
    # $DIR/tmp > $DIR/input.txt
    
    echo -n $count >> $DIR/result_hist-my_hash.csv
    echo -n , >> $DIR/result_hist-my_hash.csv
    echo -n $count >> $DIR/result_stl.csv
    echo -n , >> $DIR/result_stl.csv
    echo -n $count >> $DIR/result_hist-ideal.csv
    echo -n , >> $DIR/result_hist-ideal.csv
    echo -n $count >> $DIR/result_hist-flat_hash_fmt.csv
    echo -n , >> $DIR/result_hist-flat_hash_fmt.csv
    echo -n $count >> $DIR/result_hist-flat_hash.csv
    echo -n , >> $DIR/result_hist-flat_hash.csv
    echo -n $count >> $DIR/result_hist-flat_hash_myhashfunc.csv
    echo -n , >> $DIR/result_hist-flat_hash_myhashfunc.csv
    echo -n $count >> $DIR/result_hist-flat_hash_myhashfunc2.csv
    echo -n , >> $DIR/result_hist-flat_hash_myhashfunc2.csv
    echo -n $count >> $DIR/result_stl_myhashfunc.csv
    echo -n , >> $DIR/result_stl_myhashfunc.csv  
    echo -n $count >> $DIR/result_stl_myhashfunc2.csv
    echo -n , >> $DIR/result_stl_myhashfunc2.csv    

    for ((i=0; i < 10; i++ )); do
        g++ -o $DIR/tmp $DIR/tmp.cpp
        $DIR/tmp > $DIR/input.txt
        $DIR/hist-flat_hash
        $DIR/hist-flat_hash_myhashfunc
        $DIR/hist-flat_hash_myhashfunc2
        $DIR/hist-flat_hash_fmt
        $DIR/hist-ideal_atomdriven
        $DIR/hist-my_hash
        $DIR/edge-path_atomdriven
        $DIR/edge-path_atomdriven_myhashfunc
        $DIR/edge-path_atomdriven_myhashfunc2  
    done;
    echo  , >> $DIR/result_hist-my_hash.csv
    echo  , >> $DIR/result_stl.csv
    echo  , >> $DIR/result_stl_myhashfunc.csv
    echo  , >> $DIR/result_stl_myhashfunc2.csv
    echo  , >> $DIR/result_hist-ideal.csv
    echo  , >> $DIR/result_hist-flat_hash_fmt.csv
    echo  , >> $DIR/result_hist-flat_hash.csv
    echo  , >> $DIR/result_hist-flat_hash_myhashfunc.csv
    echo  , >> $DIR/result_hist-flat_hash_myhashfunc2.csv

    if [ $count -ge 50000 ]; then exit 0
    fi
    count=`expr $count + 5000`

done