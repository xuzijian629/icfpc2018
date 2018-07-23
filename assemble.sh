#!/bin/bash

echo "assemble"
mkdir mynbt
for file in `find ./mdl -name "FA*.mdl" |sort`
do
	./main $file
done
echo "finished!"
