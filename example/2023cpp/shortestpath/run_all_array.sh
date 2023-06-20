#!/bin/bash

# rm output2.csv
count=50


DIR='/home/imagawa/LMNtal-to-C-for-disconnected-graph/example/2023cpp/shortestpath'

# g++ -o $DIR/array $DIR/array.cpp 


rm $DIR/result_array_arr.csv
rm $DIR/result_array_list.csv
rm $DIR/result_array_stdarr.csv
rm $DIR/result_array_vec.csv

rm $DIR/result_array_list_flathash.csv
rm $DIR/result_array_list_myhash.csv
rm $DIR/result_array_vec_flathash.csv
rm $DIR/result_array_vec_flathash_myhashfunc.csv
rm $DIR/result_array_vec_myhash.csv

while true
do

    echo $count 

    sed -e 's/NUM/'$count'/g' $DIR/make_graph.cpp > $DIR/tmp.cpp
    sed -e 's/VAR/'$count'/g' $DIR/array_arr.cpp > $DIR/array_arr_tmp.cpp
    sed -e 's/VAR/'$count'/g' $DIR/array_stdarr.cpp > $DIR/array_stdarr_tmp.cpp
    sed -e 's/VAR/'$count'/g' $DIR/array_vec.cpp > $DIR/array_vec_tmp.cpp
    sed -e 's/VAR/'$count'/g' $DIR/array_list.cpp > $DIR/array_list_tmp.cpp
    sed -e 's/VAR/'$count'/g' $DIR/array_vec_flathash.cpp > $DIR/array_vec_flathash_tmp.cpp
    sed -e 's/VAR/'$count'/g' $DIR/array_vec_flathash_myhashfunc.cpp > $DIR/array_vec_flathash_myhashfunc_tmp.cpp
    sed -e 's/VAR/'$count'/g' $DIR/array_list_flathash.cpp > $DIR/array_list_flathash_tmp.cpp
    sed -e 's/VAR/'$count'/g' $DIR/array_vec_myhash.cpp > $DIR/array_vec_myhash_tmp.cpp
    sed -e 's/VAR/'$count'/g' $DIR/array_list_myhash.cpp > $DIR/array_list_myhash_tmp.cpp

    g++ -o $DIR/array_arr_tmp $DIR/array_arr_tmp.cpp  
    g++ -o $DIR/array_stdarr_tmp $DIR/array_stdarr_tmp.cpp 
    g++ -o $DIR/array_vec_tmp $DIR/array_vec_tmp.cpp 
    g++ -o $DIR/array_list_tmp $DIR/array_list_tmp.cpp
    g++ -o $DIR/array_vec_flathash_tmp $DIR/array_vec_flathash_tmp.cpp 
    g++ -o $DIR/array_vec_flathash_myhashfunc_tmp $DIR/array_vec_flathash_myhashfunc_tmp.cpp
    g++ -o $DIR/array_list_flathash_tmp $DIR/array_list_flathash_tmp.cpp
    g++ -o $DIR/array_vec_myhash_tmp $DIR/array_vec_myhash_tmp.cpp 
    g++ -o $DIR/array_list_myhash_tmp $DIR/array_list_myhash_tmp.cpp

    # g++ -o $DIR/tmp $DIR/tmp.cpp
    # $DIR/tmp > $DIR/input.txt
    
    echo -n $count >> $DIR/result_array_arr.csv
    echo -n , >> $DIR/result_array_arr.csv
    echo -n $count >> $DIR/result_array_stdarr.csv
    echo -n , >> $DIR/result_array_stdarr.csv
    echo -n $count >> $DIR/result_array_vec.csv
    echo -n , >> $DIR/result_array_vec.csv
    echo -n $count >> $DIR/result_array_list.csv
    echo -n , >> $DIR/result_array_list.csv

    echo -n $count >> $DIR/result_array_vec_flathash.csv
    echo -n , >> $DIR/result_array_vec_flathash.csv
    echo -n $count >> $DIR/result_array_vec_flathash_myhashfunc.csv
    echo -n , >> $DIR/result_array_vec_flathash_myhashfunc.csv
    echo -n $count >> $DIR/result_array_list_flathash.csv
    echo -n , >> $DIR/result_array_list_flathash.csv
    echo -n $count >> $DIR/result_array_vec_myhash.csv
    echo -n , >> $DIR/result_array_vec_myhash.csv
    echo -n $count >> $DIR/result_array_list_myhash.csv
    echo -n , >> $DIR/result_array_list_myhash.csv

    for ((i=0; i<10; i++)); do
        g++ -o $DIR/tmp $DIR/tmp.cpp
        $DIR/tmp > $DIR/input.txt
        $DIR/array_arr_tmp
        $DIR/array_stdarr_tmp
        $DIR/array_vec_tmp
        $DIR/array_list_tmp
        $DIR/array_vec_flathash_tmp
        $DIR/array_vec_flathash_myhashfunc_tmp
        $DIR/array_list_flathash_tmp
        $DIR/array_vec_myhash_tmp
        $DIR/array_list_myhash_tmp
    done

        echo  , >> $DIR/result_array_arr.csv
        echo  , >> $DIR/result_array_stdarr.csv
        echo  , >> $DIR/result_array_vec.csv
        echo  , >> $DIR/result_array_list.csv
        echo  , >> $DIR/result_array_vec_flathash.csv
        echo  , >> $DIR/result_array_vec_flathash_myhashfunc.csv
        echo  , >> $DIR/result_array_list_flathash.csv
        echo  , >> $DIR/result_array_vec_myhash.csv
        echo  , >> $DIR/result_array_list_myhash.csv

    if [ $? -eq 124 ]; then exit 0
    fi
    if [ $count -ge 300 ]; then exit 0
    fi
    count=`expr $count + 50`

done