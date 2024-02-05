#!/bin/bash

# echo -n "" > result_checkans

dir="diff"
file="graph/case/test"
output1="graph/png/flow.png"
output1="graph/png/cap.png"

n=10
        # g++ -o showgraph -O3 showgraph.cpp
        ./showgraph $file
        dot -Tpng -o $output1 tmp.dot