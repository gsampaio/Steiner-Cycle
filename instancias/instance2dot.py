#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
"""
instance2dot.py: cria uma representação do formato Graphviz dot.

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

edges = []
for i in range(0, num_edges):
    line = instance_file.readline().split()
    edges.append((line[0], line[1], line[2]))
#endfor

terminals = []
for i in range(0, num_terminals):
    terminals.append(instance_file.readline().strip())
#endfor

# Imprime no formato dot
print "graph SteinerCycle {"
print "    node [shape=box,style=filled,color=red];",
for t in terminals:
  print "%s; " % (t),

print "\n    node [shape=circle,style=rounded,color=black];"

for edge in edges:
  print "    %s -- %s [label=\"%s\"]" % (edge)

print "}"
