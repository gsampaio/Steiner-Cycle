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
	list<ListGraph::Node> * tnode = new list<ListGraph::Node>();
    ListGraph::NodeIt * v = new ListGraph::NodeIt(*graph);
	for (; *v!= INVALID; ++(*v)) {
    	if ((*terminal)[*v]) {
			tnode->push_front(*v);
		}		
	}
    delete v;

    // Then we generate a random solution for the problem
	list<ListGraph::Node> sol;
	MTRand rand;
	int i = 0;
	while (tnode->size() > 0) {
		list<ListGraph::Node>::iterator iter;
		i = rand.randInt() % tnode->size();
		int currentNode = 0;
		ListGraph::Node randNode;
		for (iter = tnode->begin(); iter != tnode->end(); iter++) {
			if (currentNode == i) {
				randNode = *iter;
				tnode->erase(iter);
				break;
			}
			currentNode++;
		}
		sol.push_back(randNode);
		currentNode = 0;
	}
    
    delete tnode;
    return sol;
}

list<ListGraph::Node> Grasp::randomSolutionWithNoise() {

	// First we instantiate an vector with all the terminal nodes
	list<ListGraph::Node> * tnode = new list<ListGraph::Node>();
    list<ListGraph::Node> * nonterm = new list<ListGraph::Node>();
    ListGraph::NodeIt * v = new ListGraph::NodeIt(*graph);
	for (; *v != INVALID; ++(*v)) {
    	if ((*terminal)[*v]) {
			tnode->push_back(*v);
		} else {
            nonterm->push_back(*v);
		}		
	}
    delete v;

    // Then we generate a random solution for the problem
	list<ListGraph::Node> sol;
	MTRand rand;
	while (tnode->size() > 0) {
        int currentNode = 0; int i= 0;
		ListGraph::Node randNode;
        list<ListGraph::Node>::iterator it;
		
	    // Add non terminal nodes to solution at a rate of 0.1
        if (rand.randExc() <= 0.1) {
            if (nonterm->size() > 0) {
                currentNode = 0;
	            i = rand.randInt() % nonterm->size();
                for (it = nonterm->begin(); it != nonterm->end(); it++) {
                    if (currentNode == i) {
                        randNode = *it;
                        nonterm->erase(it);
                        break;
                    }
                    currentNode++;
                }
                sol.push_back(randNode);
            }    
	    } else {
	        // Add the terminal nodes to the solution
    	    i = rand.randInt() % tnode->size();
    		for (it = tnode->begin(); it != tnode->end(); it++) {
    			if (currentNode == i) {
    				randNode = *it;
    				tnode->erase(it);
    				break;
    			}
    			currentNode++;
    		}
    		sol.push_back(randNode);
    		currentNode = 0;
	    }
	} 
	
    delete tnode;
    delete nonterm;
    
    return sol;    

}

ListGraph::Edge Grasp::maxWeightedEdge(list<ListGraph::Node> solution, ListGraph::EdgeMap<bool> * visited) {
    list<ListGraph::Node>::const_iterator u, v;
    double max = 0.0;
    ListGraph::Edge edge;
    ListGraph::Edge firstEdge;
    v = solution.begin();
    bool first = true;
    bool didVisited = false;
    
    // For the node from the solution we search the IncEdges
    for (unsigned int i =0; i < solution.size(); ++i) {
        u = v++;
    
        if (v == solution.end()) v = solution.begin();
    
        // Search for the valid edge
        ListGraph::IncEdgeIt e(*graph, *u);
        for (;e != INVALID; ++e) {
            if(((*graph).u(e) == *v) || ((*graph).v(e) == *v)) {
                if (first) {
                    firstEdge = e;
                    first = false;
                }                
                if (!(*visited)[e]) {
                    if ((*length)[e] > max) {
                        edge = e;
                        max = (*length)[e];
                    }
                    didVisited = true;
                }
                
            } 
        }
    }
    if (didVisited) {
        (*visited)[edge] = true;        
        return edge;
    } else {
        return firstEdge;
    }
}

list<ListGraph::Node>* Grasp::findBetterPath(list<ListGraph::Node>s, ListGraph::Edge e) {
    typedef const ListGraph::EdgeMap<double> LengthMap;
    typedef Dijkstra<FilterNodes<ListGraph>, LengthMap> ShortestPath;
    ListGraph::NodeMap<bool> * node_filter = new ListGraph::NodeMap<bool>(*graph);

    // First we create a subgraph without the nodes from solution
    ListGraph::NodeIt * v = new ListGraph::NodeIt(*graph);
    for (;*v != INVALID; ++(*v)) {
        bool belong = true;
        if (graph->id(*v) != graph->id(graph->u(e)) &&
            graph->id(*v) != graph->id(graph->v(e)) ) {
            list<ListGraph::Node>::iterator * solIt = new list<ListGraph::Node>::iterator();
            for (*solIt = s.begin(); *solIt != s.end(); (*solIt)++) {
                if (graph->id(*v) == graph->id(*(*solIt))) {
                    belong = false;
                }
            }
            delete solIt;
        }
        (*node_filter)[*v] = belong;
    }
    delete v;
    
    // Create the subgraph and then execute dijkstra
    FilterNodes<ListGraph> *subgraph = new FilterNodes<ListGraph>(*graph, *node_filter);
    ShortestPath d(*subgraph, *length);
    d.run((*subgraph).u(e));

    // Get the dijkstra result and add this on the place where was the edge
    Path<ListGraph> * p = new Path<ListGraph>(d.path((*subgraph).v(e)));
    list<ListGraph::Node> * path = new list<ListGraph::Node>();
    bool first = true;
    Path<ListGraph>::ArcIt * it = new Path<ListGraph>::ArcIt(*p);
    for (; *it != INVALID; ++(*it)) {
        ListGraph::Arc * a = new ListGraph::Arc(*it);
        ListGraph::Edge x = (*a);
        if (first) {
            path->push_back((*subgraph).u(x));
            path->push_back((*subgraph).v(x));
            first = false;
        } else {
            path->push_back((*subgraph).v(x));
        }
        delete a;
    }
    delete it;
    delete p;
    delete subgraph;
    delete node_filter;
    
    return path;
}

list<ListGraph::Node> Grasp::localSearch(list<ListGraph::Node> solution, ListGraph::EdgeMap<bool> * visited) {
    list<ListGraph::Node> new_solution;
    list<ListGraph::Node>::iterator  solution_it; 
    
    // Find a max weighted edge then change it with the minimun path 
    ListGraph::Edge e = maxWeightedEdge(solution, visited);
    list<ListGraph::Node> * path = findBetterPath(solution, e);
    for (solution_it = solution.begin(); solution_it != solution.end(); solution_it++) {
        if ((*graph).u(e) == *solution_it) {
            list<ListGraph::Node>::iterator path_it; 
            for (path_it = path->begin(); path_it != path->end(); path_it++) {
                new_solution.push_back(*path_it);
            }
        } else if ((*graph).v(e) == *solution_it) {
            continue;
        } else {
            new_solution.push_back(*solution_it);
        }
    }
    delete path;
    return new_solution;
}

double Grasp::sendSolution(list<ListGraph::Node> solution) {
    return (*delegate).didRecivedSolution(solution);
}

list<ListGraph::Node> Grasp::execute(double time) {
    Timer t;
    t.restart();
    
    // While we still have time execute
    while (t.elapsed() < time - 8) {
        unsigned int i = 0;
        ListGraph::EdgeMap<bool> * visited = new ListGraph::EdgeMap<bool>(*graph);
        initVisitedMap(visited);
        list<ListGraph::Node> solution = randomSolution();
        double value = checkBestSolution(solution);
        while (i < num_terminals) {
            double new_value = checkBestSolution(localSearch(solution,visited));
            if (value == new_value) {
                break;
            } else {
                value = new_value;
            }
            i++;
        }
        delete visited;
    }
    return best_solution;
}

void Grasp::initVisitedMap(ListGraph::EdgeMap<bool> * visited) {
    ListGraph::EdgeIt * e = new ListGraph::EdgeIt(*graph);
    for (; *e != INVALID; ++(*e)) {
        (*visited)[*e] = false;
    }
    delete e;
}

list<ListGraph::Node> Grasp::executeWithNoise(double time) {
    Timer t;
    t.restart(); 
    
    // While we still have time
    while(t.elapsed() < time - 8) {
        unsigned int i = 0;
        ListGraph::EdgeMap<bool> * visited = new ListGraph::EdgeMap<bool>(*graph);
        initVisitedMap(visited);
        list<ListGraph::Node> solution = randomSolutionWithNoise();
        double value = checkBestSolution(solution);
        while (i < num_terminals) {
            double new_value = checkBestSolution(localSearch(solution,visited));
            if (value == new_value) {
                break;
            } else {
                value = new_value;
            }
            i++;
        }
        delete visited;
    }
    return best_solution;
}

double Grasp::checkBestSolution(list<ListGraph::Node> solution) {
    // Verify the best solution to store
    double sol_value = solutionValue(solution);
    if (best_solution.size() > 0) {
        if (sol_value < solution_value) {
            best_solution = solution;
            solution_value = sol_value;
        }
    } else {
        best_solution = solution;
        solution_value = sol_value;
    }
    return sol_value;
}

double Grasp::solutionValue(list<ListGraph::Node> solution) {
    // calculate the value of the solution
    list<ListGraph::Node>::const_iterator u, v;
    double value = 0.0;
    v = solution.begin();
    
    for (unsigned int i =0; i < solution.size(); ++i) {
        u = v++;
        
        if (v == solution.end()) v = solution.begin();
        
        // Search for the valid edge
        ListGraph::IncEdgeIt e(*graph, *u);
        for (;e != INVALID; ++e) {
            if(((*graph).u(e) == *v) || ((*graph).v(e) == *v)) {
                value += (*length)[e];
            } 
        }
    }
    return value;
}
