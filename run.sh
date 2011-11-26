#!/bin/bash
# run all the instances for 2 minutes
make;
for i in `ls instances/ `; do \
    echo "Running $i";
    echo "$i:" >> out.out;
    ./RA073177.e instances/$i 120 >> out.out; 
    echo "\n" >>  out.out;\
done
