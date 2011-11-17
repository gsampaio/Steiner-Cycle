#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
"""
completa_grafo.py: completa um grafo.

(c) Carlos Eduardo de Andrade. Outubro, 2011
"""
import sys

if len(sys.argv) < 2:
    print "Usage: " + sys.argv[0] + " <instance-file>"
    sys.exit(0)

instance_file = open(sys.argv[1], "r")

data_line = instance_file.readline().split()

num_nodes = int(data_line[0].strip())
num_edges = int(data_line[1].strip())
num_terminals = int(data_line[2].strip())

maior = 0.0

edges = []
for i in range(0,  num_nodes):
    edges.append([0.0] * num_nodes)

for i in range(0, num_edges):
    line = instance_file.readline().split()
    u = int(line[0])
    v = int(line[1])

    if u > v:
        u, v = v, u

    edges[u][v] = float(line[2])

    if maior < edges[u][v]:
        maior = edges[u][v]
#endfor

terminals = []
for i in range(0, num_terminals):
    terminals.append(instance_file.readline().strip())
#endfor

###### comeco o grafo completo
num_edges = num_nodes * (num_nodes - 1) / 2
print "%d %d %d" % (num_nodes, num_edges, num_terminals)

for u in range(0, num_nodes):
    for v in range(u+1, num_nodes):
        print "%d %d %.2f" % (u, v, (edges[u][v] if edges[u][v] > 0.0 else (num_nodes * maior)))
#endfor

for t in terminals: print t
