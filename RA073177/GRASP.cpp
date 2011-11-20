#include "GRASP.hpp"
#include "../mtrand.hpp"
#include "../timer.hpp"

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
		////cout << (*graph).id(*iter) << " ";
	}
    ////cout << endl;
    return sol;
}

list<ListGraph::Node> Grasp::randomSolutionWithNoise() {

	// First we instantiate an vector with all the terminal nodes
	list<ListGraph::Node> tnode;
    list<ListGraph::Node> nonterm;
	for (ListGraph::NodeIt v(*graph); v!= INVALID; ++v) {
    	if ((*terminal)[v]) {
			tnode.push_back(v);
		} else {
            nonterm.push_back(v);
		}		
	}

    // Then we generate a random solution for the problem
	list<ListGraph::Node> sol;
	MTRand rand;
	while (tnode.size() > 0) {
        int currentNode = 0; int i= 0;
		ListGraph::Node randNode;
		list<ListGraph::Node>::iterator it;
		
	    // Add non terminal nodes to solution at a rate of 0.1
        if (rand.randExc() <= 0.1) {
            if (nonterm.size() > 0) {
                currentNode = 0;
	            i = rand.randInt() % nonterm.size();
                list<ListGraph::Node>::iterator it;
                for (it = nonterm.begin(); it != nonterm.end(); it++) {
                    if (currentNode == i) {
                        randNode = *it;
                        nonterm.erase(it);
                        break;
                    }
                    currentNode++;
                }
                sol.push_back(randNode);
            }    
	    } else {
    	    i = rand.randInt() % tnode.size();
    		for (it = tnode.begin(); it != tnode.end(); it++) {
    			if (currentNode == i) {
    				randNode = *it;
    				tnode.erase(it);
    				break;
    			}
    			currentNode++;
    		}
    		sol.push_back(randNode);
    		currentNode = 0;
	    }
	}    
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
                bool found = false;
                list<ListGraph::Edge>::iterator eit;
                for (eit = removed.begin(); eit != removed.end(); eit++){
                    if (*eit == e) {
                        found = true;
                    } 
                }
                if (!found) break;
            }

        }
        
        if ((*length)[e] > max) {
            max = (*length)[e];
            maxEdge = e;
        }
    }
    removed.push_back(maxEdge);
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
        ////cout << "Nó atual " << (*graph).id(v) << endl;
        if(((*graph).u(e) != v) && ((*graph).v(e) != v)) {
            list<ListGraph::Node>::iterator iter;
            for (iter = s.begin(); iter != s.end(); iter++) {
                ////cout << "\tatual da solucao = " << (*graph).id(*iter) <<endl;
                if((*graph).id(v) == (*graph).id(*iter)) {
                    ////cout << "\t\titem " << (*graph).id(v) << " removido" << endl;
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
    
    ////cout << "NEW SOLUTION: ";
    list<ListGraph::Node>::iterator it;
    for (it = new_solution.begin(); it != new_solution.end(); it++) {
        ////cout << (*graph).id(*it) << " ";
    }
    ////cout << endl;
    
    return new_solution;
}

double Grasp::sendSolution(list<ListGraph::Node> solution) {
    return (*delegate).didRecivedSolution(solution);
}

void Grasp::execute(double time) {
    list<ListGraph::Node>last_solution;
    //cout << "Executing with no noise add" << endl;
    Timer t;
    t.restart();
    

    // Enquanto temos tempo executa
    while (t.elapsed() < time - 2) {
        unsigned int i = 0;
        list<ListGraph::Node> solution = randomSolution();
        double value = sendSolution(solution);
        while (i < num_terminals) {
            double new_value = sendSolution(localSearch(solution));
            if (value == new_value) {
                break;
            } else {
                value = new_value;
            }
            i++;
        }

    }
}

void Grasp::executeWithNoise(double time) {
    //cout << "Executing with probabilistic noise" << endl;
    Timer t;
    t.restart(); 
    // Enquanto temos tempo executa 
    while(t.elapsed() < time - 2) {
        unsigned int i = 0;
        list<ListGraph::Node> solution = randomSolutionWithNoise();
        double value = sendSolution(solution);
        while (i < num_terminals) {
            double new_value = sendSolution(localSearch(solution));
            if (value == new_value) {
                break;
            } else {
                value = new_value;
            }
            i++;
        }
    }
}
