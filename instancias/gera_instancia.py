#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
"""
gera_instancia.py: gera uma instancia do Steiner Cuycle Problem

(c) Carlos Eduardo de Andrade. Outubro, 2011
"""
import sys, random, time

# Constantes
random.seed(int(time.time()))

#-----------------------------------------------------------------------------#

class Custos(object):
    random_cost = False

    @staticmethod
    def MENOR_CUSTO():
        return random.random() if Custos.random_cost else 1.0

    @staticmethod
    def MAIOR_CUSTO():
        return 2 * Custos.MENOR_CUSTO() + (0 if Custos.random_cost else 1)
#endclass

#-----------------------------------------------------------------------------#

def InternalExternalCycle(num_nodes, type = "I", dot_format = False):
    num_edges = num_nodes * (num_nodes - 1) / 2
    taken_edges = dict()

    if not dot_format:
        print "%d %d %d" % (num_nodes, num_edges, num_nodes/2)
    else:
        print "graph {"
        print "    node [shape=circle]",
        for i in xrange(1, num_nodes, 2):
            print "%d;" % (i),

        print "\n    node [shape=box,style=filled,color=red]; ",
        for i in xrange(0, num_nodes, 2):
            print "%d;" % (i),

        print "\n"
    #endif

    # Checamos se é alternante
    alternating, type = (True, "E") if type == "A" else (False, type)
    output_string = "%d %d %.2f"

    if dot_format:
        output_string = "    %d -- %d [label=\"%.2f\"]"

    for u in xrange(0, num_nodes):
        phase = ((u % 4) == 0)
        for v in xrange(u+1, num_nodes):
            # circulo externo
            if u+1 == v:
                print output_string % (u, v, (Custos.MENOR_CUSTO() if type == "E" else Custos.MAIOR_CUSTO()))

            # circulo interno
            elif u % 2 == 0 and v-u == 2:
                custo = Custos.MAIOR_CUSTO() if type == "E" else Custos.MENOR_CUSTO()

                if alternating and (phase or (v-u == num_nodes-2 and ((num_nodes / 2) % 2 == 0))):
                    custo = Custos.MENOR_CUSTO()
                    phase = False

                print output_string % (u, v, custo)

            # entre terminais
            elif u % 2 == 0 and v % 2 == 0:
                print output_string % (u, v, Custos.MAIOR_CUSTO() if type == "E" else Custos.MENOR_CUSTO())

            # primeiro e ultimo
            elif u == 0 and v == num_nodes - 1:
                print output_string % (u, num_nodes - 1, (Custos.MENOR_CUSTO() if type == "E" else Custos.MAIOR_CUSTO()))

            # restante do grafo
            else:
                custo = (Custos.MAIOR_CUSTO() if type == "E" else Custos.MENOR_CUSTO()) * num_nodes
                print output_string % (u, v, custo)
    #endfor
    #print output_string % (num_nodes-1, 0, (Custos.MENOR_CUSTO() if type == "E" else Custos.MAIOR_CUSTO()))

    if dot_format:
        print "}"
    else:
        for u in xrange(0, num_nodes, 2): print u
#InternalCycle


def main():
    if len(sys.argv) < 4:
        print "Usage: " + sys.argv[0] + " <gen_type> <random> <num_nodes>"
        print "where <gen_type> is"
        print "\t- IC: internal cycle (use only edges between terminals)"
        print "\t- EC: external cycle (not use edges between terminals)"
        print "\t- AC: alternating cycle (use a edge between terminals follow by edges between a terminal and a steiner node)"
        print "<random> is"
        print "\t- R: generate random costs for edges such that E[sum edges S-T] = E[sum edges T-T]"
        print "\t- F: fixed costs to the edges."
        sys.exit(0)
    #endif

    gen_type = sys.argv[1].upper()
    Custos.random_cost = (sys.argv[2].upper()[0] == "R")
    num_nodes = int(sys.argv[3])

    #print gen_type, Custos.random_cost, num_nodes

    if gen_type in ["IC", "EC", "AC"]:
        InternalExternalCycle(num_nodes, type = gen_type[0], dot_format = False)

#endmain


if __name__ == "__main__":
    main()

