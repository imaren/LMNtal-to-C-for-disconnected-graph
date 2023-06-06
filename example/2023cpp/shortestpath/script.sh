#!/bin/bash
new_line=`cat -`
cat shortestpath.lmn | sed -e "s/VAR/${new_line}/g"