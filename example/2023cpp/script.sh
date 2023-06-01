#!/bin/bash
new_line=`cat -`
cat shortestpath.txt | sed -e "s/VAR/${new_line}/g"