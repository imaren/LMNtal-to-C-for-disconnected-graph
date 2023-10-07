#!/bin/bash

num=250

./make_graph $num > tmp_input
tail -n 1 tmp_input > tmp_lmn
sed -e '$d' tmp_input > tmp_cpp

cat time-point.lmn > tmp.lmn
cat tmp_lmn >> tmp.lmn
slim tmp.lmn