#include "GRASP.hpp"
#include "../mtrand.hpp"

#include <lemon/list_graph.h>
#include <lemon/adaptors.h>
#include <lemon/dijkstra.h>
using namespace lemon;

Grasp::Grasp(GraspDelegate * d,
             ListGraph &g, 
             ListGraph::NodeMap<bool> &t, 
             ListGraph::EdgeMap<double> &l,
             unsigned int nt) {
    delegate = d,
    graph = &g;
    terminal = &t;
    length = &l;
    num_terminals = nt;
}

list<ListGraph::Node> Grasp::randomSolution() {
    
	// First we instantiate an vector with all the terminal nodes
	list<ListGraph::Node> tnode;
	for (ListGraph::NodeIt v(*graph); v!= INVALID; ++v) {
    	if ((*terminal)[v]) {
			tnode.push_front(v);
		}		
	}

    // Then we generate a random solution for the problem
	list<ListGraph::Node> sol;
	MTRand rand;
	int i = 0;
	while (tnode.size() > 0) {
		list<ListGraph::Node>::iterator iter;
		i = rand.randInt() % tnode.size();
		int currentNode = 0;
		ListGraph::Node randNode;
		for (iter = tnode.begin(); iter != tnode.end(); iter++) {
			if (currentNode == i) {
				randNode = *iter;
				tnode.erase(iter);
				break;
			}
			currentNode++;
		}
		sol.push_back(randNode);
		currentNode = 0;
	}
	list<ListGraph::Node>::iterator iter;
	for(iter = sol.begin(); iter != sol.end(); iter++) {
		cout << (*graph).id(*iter) << " ";
	}
    cout << endl;
    return sol;
}

ListGraph::Edge Grasp::maxWeightedEdge(list<ListGraph::Node> solution) {
    list<ListGraph::Node>::const_iterator u, v;
    double max = 0.0;
    v = solution.begin();
    ListGraph::Edge maxEdge;
    for (unsigned int i =0; i < solution.size(); ++i) {
        u = v++;
        
        if (v == solution.end()) v = solution.begin();
        
        // Search for the valid edge
        ListGraph::IncEdgeIt e(*graph, *u);
        for (;e != INVALID; ++e) {
            if(((*graph).u(e) == *v) || ((*graph).v(e) == *v)) {
                break;
            } 
        }
        
        if ((*length)[e] > max) {
            max = (*length)[e];
            maxEdge = e;
        }
    }
    
    return maxEdge;
}

list<ListGraph::Node> Grasp::findBetterPath(list<ListGraph::Node> s, ListGraph::Edge e) {
    
    typedef const ListGraph::EdgeMap<double> LengthMap;
    typedef Dijkstra<SubGraph<ListGraph>, LengthMap> ShortestPath;
    
    ListGraph::NodeMap<bool> node_filter(*graph);
    ListGraph::EdgeMap<bool> edge_filter(*graph);
    list<ListGraph::Node> nodesToRemove;
    
    // Create the node_filter
    for (ListGraph::NodeIt v(*graph); v != INVALID; ++v) {
        cout << "Nó atual " << (*graph).id(v) << endl;
        if(((*graph).u(e) != v) && ((*graph).v(e) != v)) {
            list<ListGraph::Node>::iterator iter;
            for (iter = s.begin(); iter != s.end(); iter++) {
                cout << "\tatual da solucao = " << (*graph).id(*iter) <<endl;
                if((*graph).id(v) == (*graph).id(*iter)) {
                    cout << "\t\titem " << (*graph).id(v) << " removido" << endl;
                    nodesToRemove.push_back(v);
                    node_filter[v] = false;
                    break;
                } else {
                    node_filter[v] = true;
                }
            }
        } else {
            node_filter[v] = true;
        }
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
    
    
    //FilterNodes<ListGraph> sg(graph, node_filter); 
    SubGraph<ListGraph> sg(*graph, node_filter, edge_filter);
    ShortestPath d(sg, *length);
    d.run(sg.u(e));
    Path<ListGraph> p = d.path(sg.v(e));
    list<ListGraph::Node> path;
    bool first = true;
    for (Path<ListGraph>::ArcIt it(p); it != INVALID; ++it) {
        ListGraph::Arc a = it;
        ListGraph::Edge x = a;
        if (first) {
            path.push_front(sg.u(x));
            path.push_front(sg.v(x));
            first = false;
        } else {
            path.push_front(sg.v(x));
        }
    }
    
    path.reverse();
    cout << "PATH: ";
    list<ListGraph::Node>::iterator it;
    for (it = path.begin(); it != path.end(); it++) {
        cout << sg.id(*it) << " ";
    }
    cout << endl;
    
    return path;
}

list<ListGraph::Node> Grasp::localSearch(list<ListGraph::Node> solution) {
    list<ListGraph::Node> new_solution;
    list<ListGraph::Node>::iterator solution_it; 
    
    ListGraph::Edge e = maxWeightedEdge(solution);
    list<ListGraph::Node> path = findBetterPath(solution, e);
    for (solution_it = solution.begin(); solution_it != solution.end(); solution_it++) {
        if ((*graph).u(e) == *solution_it) {
            list<ListGraph::Node>::iterator path_it; 
            for (path_it = path.begin(); path_it != path.end(); path_it++) {
                new_solution.push_back(*path_it);
            }
        } else if ((*graph).v(e) == *solution_it) {
            continue;
        } else {
            new_solution.push_back(*solution_it);
        }
    }
    
    cout << "NEW SOLUTION: ";
    list<ListGraph::Node>::iterator it;
    for (it = new_solution.begin(); it != new_solution.end(); it++) {
        cout << (*graph).id(*it) << " ";
    }
    cout << endl;
    
    return new_solution;
}

bool Grasp::sendSolution(list<ListGraph::Node> solution) {
    return (*delegate).didRecivedSolution(solution);
}

void Grasp::execute() {
    unsigned int i = 0;
    ListGraph::Edge e;
    list<ListGraph::Node> solution = randomSolution();
    bool troca = sendSolution(solution);
    while (troca && i < num_terminals) {
        troca = sendSolution(localSearch(solution));
        i++;
    }
}
