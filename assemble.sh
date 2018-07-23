#!/bin/bash

echo "assemble"
for file in `find ./mdl -name "FA*.mdl" |sort`
do
	./main $file
done
echo "finished!"
