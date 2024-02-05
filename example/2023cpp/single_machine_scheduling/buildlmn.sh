#!/bin/bash

i=2
j=1

cat single_machine_scheduling_simple.lmn > tmps.lmn
cat testcase/L$i-$j >> tmps.lmn

cat single_machine_scheduling_simple_onlycost.lmn > tmpc.lmn
cat testcase/L$i-$j >> tmpc.lmn

cat single_machine_scheduling_simple_nd.lmn > tmpn.lmn
cat testcase/Lnd$i-$j >> tmpn.lmn