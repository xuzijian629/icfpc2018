#!/bin/bash
ulimit -s $((2**15))
echo "連結成分の有無のチェック"
sleep 1
for file in `find ./mdl -name "*.mdl" |sort`
do
    ./check_connected $file
done 
echo ""
echo "垂れ下がりの有無のチェック"
sleep 1
for file in `find ./mdl -name "*.mdl" |sort`
do
    ./check_connected $file UPPER
done 
