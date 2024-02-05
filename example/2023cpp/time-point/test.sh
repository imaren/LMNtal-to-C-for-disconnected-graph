#!/bin/bash

echo , > tmp.csv

num=50
max=600
# ver=1
for((i=50; i<= $max; i+=50)); do
for ver in {1,2,3}; do
# cat time-point.lmn > tmp.lmn
# cat testcase/L2_$i-$ver >> tmp.lmn
# /usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 tmp.lmn > tmp.il
# valgrind --tool=massif --pages-as-heap=yes --massif-out-file=./massif/massif.L2_$i-$ver slim tmp.il
# (/usr/bin/time -f "%M,%E" slim tmp.il) 2>> mem_lmn.csv
valgrind --tool=massif --pages-as-heap=yes --massif-out-file=./massif/delfirst_C2_$i-$ver ./delfirst_algo testcase/C2_$i-$ver
# valgrind --tool=massif --pages-as-heap=yes --massif-out-file=./massif/set_C2_$i-$ver ./mem_set testcase/C2_$i-$ver
# echo $i,$ver
# ./memreduce testcase/C2_$i-$ver >> tmp.csv

# (/usr/bin/time -f "%M,%E" ./flat_noprint testcase/C2_$i-$ver) 2>> mem_cpp.csv
# (/usr/bin/time -f "%M,%E" ./memreduce testcase/C2_$i-$ver) 2>> mem_cpp_reduce.csv
# (/usr/bin/time -f "%M,%E" ./mem_set testcase/C2_$i-$ver) 2>> mem_set.csv
# (/usr/bin/time -f "%M,%E" ./mem_specific testcase/C2_$i-$ver) 2>> mem_specific.csv
done
done