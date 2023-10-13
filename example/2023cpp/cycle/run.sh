#!/bin/bash

DIR="/home/imagawa/LMNtal-to-C-for-disconnected-graph/example/2023cpp/cycle"

cat ${DIR}/cycle.lmn > ${DIR}/tmp.lmn
cat ${DIR}/input >> ${DIR}/tmp.lmn


slim ${DIR}/tmp.lmn