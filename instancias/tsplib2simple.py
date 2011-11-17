#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
"""
tsplib2simple.py: torna mais simples o formato da tsplib. Somente p/
distâncias euclidianas.

(c) Carlos Eduardo de Andrade. Outubro, 2011
"""
import sys, itertools, math

###############################################################################
# contantes
PI = 3.141592
RRR = 6378.388

###############################################################################

def latitude_longitude(valor):
    deg = int(valor)
    min = valor - deg
    return PI * (deg + 5.0 * min / 3.0) / 180.0

###############################################################################

if len(sys.argv) < 2:
    print "Usage: " + sys.argv[0] + " <tsplib-file>"
    sys.exit(0)

tsplib_lines = itertools.chain(open(sys.argv[1], "r").readlines())

num_nodes = 0
num_edges = 0
edge_weight_type = ""
edges = []

line = tsplib_lines.next().split()
while line[0] != "NODE_COORD_SECTION":
    if line[0] == "DIMENSION:":
        num_nodes = int(line[1])

    if line[0] == "EDGE_WEIGHT_TYPE:":
        edge_weight_type = line[1].strip()

    line = tsplib_lines.next().split()
#endwhile

num_edges = num_nodes * (num_nodes - 1) / 2

line = tsplib_lines.next().split()
points = []
while line[0] != "EOF":
    points.append((float(line[1]), float(line[2])))
    line = tsplib_lines.next().split()
#endwhile

# escreve no formato simples
print "%d %d %d" % (num_nodes, num_edges, num_nodes)

for i in xrange(0, num_nodes):
    for j in xrange(i+1, num_nodes):

        if edge_weight_type == "EUC_2D":
            xd = points[i][0] - points[j][0]
            yd = points[i][1] - points[j][1]
            dist = int(math.sqrt(xd**2 + yd**2))

            print "%d %d %d" % (i, j, dist)
        #endif

        if edge_weight_type == "GEO":
            latitude_i = latitude_longitude(points[i][0])
            latitude_j = latitude_longitude(points[j][0])
            longitude_i = latitude_longitude(points[i][1])
            longitude_j = latitude_longitude(points[j][1])

            q1 = math.cos(longitude_i - longitude_j)
            q2 = math.cos(latitude_i - latitude_j)
            q3 = math.cos(latitude_i + latitude_j)

            dist = int(RRR * math.acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0)
            print "%d %d %d" % (i, j, dist)
        #endif

#endfors

for i in xrange(0, num_nodes):
    print i
