#include "Greedy.hpp"
#include "../mtrand.hpp"

#define DBL_MAX 1.7976931348623158e+308 /* max value */

using namespace lemon;

GreedyHeuristic::GreedyHeuristic(ListGraph &g, 
             ListGraph::NodeMap<bool> &t, 
             ListGraph::EdgeMap<double> &l,
             unsigned int nt) {

    graph = &g;
    terminal = &t;
    length = &l;
    num_terminals = nt;                 
}

list<ListGraph::Node> GreedyHeuristic::execute() {
    
    typedef ListGraph::EdgeMap<double> LengthMap;
    typedef Dijkstra<SubGraph<ListGraph>, LengthMap> ShortestPath;
    
    list<ListGraph::Node> solution;    
    // First we instantiate an vector with all the terminal nodes
    list<ListGraph::Node> tnode;
    for (ListGraph::NodeIt v(*graph); v!= INVALID; ++v) {
        if ((*terminal)[v]) {
            tnode.push_back(v);
        }
    }

    // Random get an terminal node 
    MTRand rand;
    list<ListGraph::Node>::iterator iter;
    int i = rand.randInt() % tnode.size();
    int currentNode = 0;
    ListGraph::Node node;
    for (iter = tnode.begin(); iter != tnode.end(); iter++) {
        if (currentNode == i) {
            node = *iter;
            tnode.erase(iter);
            break;
        }
        currentNode++;
    }
    solution.push_back(node);
    while (tnode.size() > 0) {
        double min = DBL_MAX;        
        list<ListGraph::Node>::iterator term;
        list<ListGraph::Node>::iterator bestNode;
        Path<ListGraph> p;
        
        // First we instantiate an vector with all the terminal nodes
        list<ListGraph::Node> fulltnode;
        for (ListGraph::NodeIt v(*graph); v!= INVALID; ++v) {
            if ((*terminal)[v]) {
                fulltnode.push_back(v);
            }
        }
        //cout << "Saindo de: " << (*graph).id(node);        
        for (term = tnode.begin(); term != tnode.end(); term++) {
            ListGraph::NodeMap<bool> node_filter(*graph);
            ListGraph::EdgeMap<bool> edge_filter(*graph);
            //cout << " Chegando em: " << (*graph).id(*term) << endl;
            list<ListGraph::Node> nodesToRemove;
            // Create the node_filter
            for (ListGraph::NodeIt v(*graph); v != INVALID; ++v) {
                //cout << "No atual: " << (*graph).id(v) << endl;
                bool belongSubGraph = true;
                if(( (*graph).id(v) != (*graph).id(node)) &&
                    ((*graph).id(v) != (*graph).id(*term))) {
                    list<ListGraph::Node>::iterator iter;

                    // Remove all the nodes that belongs to the solution
                    for (iter = solution.begin(); iter != solution.end(); iter++) {
                        if((*graph).id(v) == (*graph).id(*iter)) {
                            nodesToRemove.push_back(v);
                            belongSubGraph = false;
                            //cout << "\t no " << (*graph).id(*iter) << "removido pertence a solucao" << endl;
                            break;
                        }
                    }

                    // Remove all the other terminal nodes 
                    for (iter = fulltnode.begin(); iter != fulltnode.end(); iter++) {
                        if((*graph).id(v) == (*graph).id(*iter)) {
                            nodesToRemove.push_back(v);
                            belongSubGraph = false;
                            //cout << "\t no " << (*graph).id(*iter) << "removido pertence aos terminais" << endl;
                            break;
                        }
                    }
                }
                node_filter[v] = belongSubGraph;
            }

            // Create the edge_filter
            for (ListGraph::EdgeIt ed((*graph)); ed != INVALID; ++ed) {
                list<ListGraph::Node>::iterator iter;
                bool value = true;
                for (iter = nodesToRemove.begin(); iter != nodesToRemove.end(); iter++) {
                    if ( (*graph).u(ed) == *iter || (*graph).v(ed) == *iter) {
                        value = false;
                    }
                }
                edge_filter[ed] = value;
            }

            // Generate a subgraph with the nodes that arent on the solution
            SubGraph<ListGraph> sg(*graph, node_filter, edge_filter);
            ShortestPath d(sg, *length);            
            d.run(node);
            double current = calculatePathValue(d.path(*term));
            if (current < min) {
                min = current; 
                bestNode = term;
                p = d.path(*term);
            }
            //cout << "VALUE: " << current << " PATH: " << (*graph).id(node)<< " ";
            for (Path<ListGraph>::ArcIt it(p); it != INVALID; ++it) {
                ListGraph::Arc a = it;
                ListGraph::Edge x = a;
                if ((*graph).u(x) == node) {
                    //cout << (*graph).id((*graph).v(x)) << " ";
                } else if ((*graph).v(x) == node) {
                    //cout << (*graph).id((*graph).u(x)) << " ";
                }
            }
            //cout << endl;
        }
        tnode.erase(bestNode);
        
        // Add the shortest path to the solution
        bool first = true;
        //cout << "PATH: " << endl;
        for (Path<ListGraph>::ArcIt it(p); it != INVALID; ++it) {
            ListGraph::Arc a = it;
            ListGraph::Edge x = a;
            //cout << "edge: " << (*graph).id((*graph).u(x)) << "->" << (*graph).id((*graph).v(x)) << endl;
            if ((*graph).u(x) == node) {
                solution.push_back((*graph).v(x));
            } else if ((*graph).v(x) == node) {
                solution.push_back((*graph).u(x));                    
            } else {
                solution.push_back((*graph).v(x));
            }

            //cout << "\t add 1 nodes:" << endl;;
            //cout << "\t\t add " << (*graph).id((*graph).v(x)) << endl;
        }
        
        //cout << "SOLUCAO ATUAL: ";
        list<ListGraph::Node>::iterator s;
        for (s = solution.begin(); s != solution.end(); s++) {
            //cout << (*graph).id(*s) << " ";
        }
        //cout << endl;

		// Get the last element from solution
        term = solution.end();
        term--; node = *term;
	}
	
    return solution;
}

double GreedyHeuristic::calculatePathValue(Path<ListGraph> p) {
    double resp = 0;
    for (Path<ListGraph>::ArcIt it(p); it != INVALID; ++it) {
        ListGraph::Arc a = it;
        ListGraph::Edge x = a;
        resp += (*length)[x];
    }
    return resp;
}
