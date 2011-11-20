#ifndef GREEDY_HPP
#define GREEDY_HPP

#include <iostream>
using namespace std;

#include <lemon/list_graph.h>
#include <lemon/adaptors.h>
#include <lemon/dijkstra.h>
using namespace lemon;

class GreedyHeuristic {
    
    public:
        GreedyHeuristic(ListGraph &g, 
              ListGraph::NodeMap<bool> &t, 
              ListGraph::EdgeMap<double> &l,
              unsigned int nt);
        SubGraph<ListGraph> generateSubGraph(list<ListGraph::Node> solution,
              ListGraph::Node currentNode);
        list<ListGraph::Node> execute();
        double calculatePathValue(Path<ListGraph> p);
        
    
    protected:
        ListGraph * graph;
        ListGraph::NodeMap<bool> * terminal;
        ListGraph::EdgeMap<double> * length;
        unsigned int num_terminals;
};
#endif 