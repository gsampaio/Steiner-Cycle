#!/bin/bash
# run all the instances for 2 minutes
make;
for i in `ls instances`; do \
    echo "Runing $i";
    ./RA073177.e instances/$i 120 >> out.out; \
done