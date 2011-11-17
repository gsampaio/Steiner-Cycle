#!/bin/bash
###############################################################################
# Run BRKGA on Combinatorial Auction Instances
#
# Author: Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
#
# (c) Copyright 2011 Institute of Computing, University of Campinas.
#     All Rights Reserved.
#
#  Created on : Oct 06, 2011 by andrade
#  Last update: Oct 06, 2011 by andrade
#
# This software is licensed under the Common Public License. Please see
# accompanying file for terms.
###############################################################################

# Where we need put the generated instances
INSTANCES_DIR="simple"

# Types of instances to be generated
# - IC: internal cycle (use only edges between terminals)
# - EC: external cycle (not use edges between terminals)
# - AC: alternating cycle (use a edge between terminals follow by edges between a terminal and a steiner node)
TYPES="IC EC AC"

# Sizes
SIZES="8 10 28 30 200 202"

##############################################################################

if [ ! -e $INSTANCES_DIR ]
then
    mkdir $INSTANCES_DIR
fi

for size in $SIZES
do
    for type in $TYPES
    do
        echo
        formated_size=$(printf "%03d" $size) ;
        echo "gera_instancia.py $type Random $size >" \
             "$INSTANCES_DIR/${type}_${formated_size}.scp"

        ./gera_instancia.py $type Fixed $size > \
            $INSTANCES_DIR/${type}_${formated_size}.scp
    done
done
