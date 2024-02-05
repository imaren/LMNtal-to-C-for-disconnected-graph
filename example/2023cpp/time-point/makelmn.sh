#!/bin/bash
i=2
j=50
k=1


cat time-point.lmn > tmp2.lmn
cat testcase/L$i\_$j-$k >> tmp2.lmn
/usr/local/bin/lmntal-compiler/bin/lmntal --slimcode -O3 --hl-opt tmp2.lmn > tmp.il
