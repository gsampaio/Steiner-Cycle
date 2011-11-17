#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
"""
steinlib2simple.py: torna mais simples o formato da steinlib

(c) Carlos Eduardo de Andrade. Outubro, 2011
"""
import sys

if len(sys.argv) < 2:
    print "Usage: " + sys.argv[0] + " <steinlib-file>"
    sys.exit(0)

steinlib_lines = open(sys.argv[1], "r").readlines()

# joga fora cabeçalho
line_number = 0
section = ""
while(section.upper() != "GRAPH"):
    line = steinlib_lines[line_number].split()

    if(len(line) > 1):
        section = line[1].strip()

    line_number += 1
#endwhile

# le arestas
num_nodes = 0
num_edges = 0
edges = []
while(line[0] != "END"):
    line = [x.strip().upper() for x in steinlib_lines[line_number].split()]

    if(line[0] == "NODES"):
        num_nodes = line[1]

    if(line[0] == "EDGES"):
        num_edges = line[1]

    if(line[0] == "E"):
        edges.append((int(line[1])-1, int(line[2])-1, line[3]))

    line_number += 1
#endwhile

# joga fora mais umas coisas
section = ""
while(section.upper() != "TERMINALS"):
    line = steinlib_lines[line_number].split()

    if(len(line) > 1):
        section = line[1].strip()

    line_number += 1
#endwhile

# le terminais
num_terminals = 0
terminals = []
while(line[0] != "END"):
    line = [x.strip().upper() for x in steinlib_lines[line_number].split()]

    if(line[0] == "TERMINALS"):
        num_terminals = line[1]

    if(line[0] == "T"):
        terminals.append(int(line[1])-1)

    line_number += 1
#endwhile

# imprime no novo formato
print "%s %s %s" % (num_nodes, num_edges, num_terminals)

for edge in edges:
    print "%d %d %s" % (edge)

for terminal in terminals:
    print terminal
