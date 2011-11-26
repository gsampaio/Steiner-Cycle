#include "GRASP.hpp"
#include "../mtrand.hpp"
#include "../timer.hpp"

#include <lemon/list_graph.h>
#include <lemon/adaptors.h>
#include <lemon/dijkstra.h>

using namespace lemon;

#define DBL_MAX 1.7976931348623158e+308 /* max value */


// Constructor
Grasp::Grasp(GraspDelegate * d,
             ListGraph &g, 
             ListGraph::NodeMap<bool> &t, 
             ListGraph::EdgeMap<double> &l,
             unsigned int nt,
             unsigned int nn) {
    delegate = d,
    graph = &g;
    terminal = &t;
    length = &l;
    num_terminals = nt;
    node_size = nn;
}

// Heurstic for generate random solution only with terminal nodes
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

// Greed solution for the problem
list<ListGraph::Node> Grasp::greedRandomSolution() {
    // First we instantiate an vector with all the terminal nodes
	list<ListGraph::Node> * tnode = new list<ListGraph::Node>();
    ListGraph::NodeIt * v = new ListGraph::NodeIt(*graph);
	for (; *v!= INVALID; ++(*v)) {
    	if ((*terminal)[*v]) {
			tnode->push_front(*v);
		}		
	}
    delete v;
	
	// Then we get an random terminal node
	list<ListGraph::Node> sol;
	MTRand rand;
	list<ListGraph::Node>::iterator iter;
	int i = rand.randInt() % tnode->size();
	int currentNode = 0;
	ListGraph::Node node;
	for (iter = tnode->begin(); iter != tnode->end(); iter++) {
		if (currentNode == i) {
            node = *iter;
			tnode->erase(iter);
			break;
		}
		currentNode++;
	}
	
    delete tnode;
	
    ListGraph::NodeMap<bool> visited(*graph);
    ListGraph::NodeIt nit(*graph);
    for (; nit != INVALID; ++nit) {
        visited[nit] = false;
    } 
    visited[node] = true;
    ListGraph::Node best;
    unsigned int nterm = 1;
    
    // We mainly seek for the best edges and make an solution choosing the bests
    // nodes
    list<ListGraph::Node> solution;
    solution.push_back(node);
	while(nterm < num_terminals) {
        double min = DBL_MAX;
	    ListGraph::IncEdgeIt e(*graph, node);
        for (;e != INVALID; ++e) {
            double current;
            ListGraph::Node v;
            if(node == graph->u(e)) {
                v = graph->v(e);
            } else {
                v = graph->u(e);
            }
            // Open only not visited edges 
            if (!visited[v]) {
                if ((*terminal)[v]) {
                    current = (*length)[e]/2;
                } else {
                    current = (*length)[e];
                }
                if (current < min) {
                    min = current;
                    best = v;
                }
            }
        }
        solution.push_back(best);
        if ((*terminal)[best]) nterm++;
        node = best;
        visited[best] = true;
	}
    return solution;
}


// Generate an random solution with a ramdom number of terminal nodes
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
    double porcentage = rand.rand();
	while (tnode->size() > 0) {
        int currentNode = 0; int i= 0;
		ListGraph::Node randNode;
        list<ListGraph::Node>::iterator it;
		
	    // Add non terminal nodes to solution at a random rand
        if (rand.randExc() <= porcentage) {
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

// Return a random edge from the solution
ListGraph::Edge Grasp::randomEdge(list<ListGraph::Node> solution) {
    ListGraph::Edge edge;
    ListGraph::Node a,b;
    MTRand random;
    unsigned int randInt = random.randInt() % (solution.size() - 2);
    unsigned int x = 0;
    list<ListGraph::Node>::iterator it;
    for (it = solution.begin(); it != solution.end(); it++) {
        if (x == randInt) {
            a = *it;
        } else if (x == randInt + 1) {
            b = *it;
        }
        x++;
    }
    edge = findEdge(*graph, a, b);

    return edge;
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

list<ListGraph::Node> Grasp::twoopt(list<ListGraph::Node> s) {
    list<ListGraph::Node> new_solution;
    ListGraph::Edge a;
    ListGraph::Edge b;
    
    // Make sure we have two diferent edges 
    do {
        a = randomEdge(s);
        b = randomEdge(s);
    } while (a != b);
    
    double cost_ab = (*length)[a] + (*length)[b];
    
    ListGraph::Node au = graph->u(a);
    ListGraph::Node av = graph->v(a);
    ListGraph::Node bu = graph->u(b);
    ListGraph::Node bv = graph->v(b);
    
    // first case 
    ListGraph::Edge aubu = findEdge(*graph, au, bu);
    ListGraph::Edge avbv = findEdge(*graph, av, bv);
    double cost_aubu_avbv = (*length)[aubu] + (*length)[avbv];
    
    // secound case
    ListGraph::Edge aubv = findEdge(*graph, au, bv);
    ListGraph::Edge avbu = findEdge(*graph, av, bu);
    double cost_aubv_avbu = (*length)[aubv] + (*length)[avbu];
    
    if (cost_aubu_avbv < cost_ab && cost_aubu_avbv < cost_aubv_avbu) {
        //  First case is better 
        list<ListGraph::Node>::iterator it;
        list<ListGraph::Node>::iterator auit;
        
        // Verify if the path will continue to make an cycle 

        for (it = s.begin(); it != s.end(); it++) {}
        
        // First we seek for Au node
        int count_au = 0;
        for (it = s.begin(); it != s.end(); it++) {
            if (au == *it) {
                auit = it;
                break;
            }
            count_au++;
        }
        
        // From the rest of the path we search for the Bu node
        int count_bu = 0;
        list<ListGraph::Node>::iterator buit;
        for (it = s.begin(); it != s.end(); it++) {
            if (bu == *it) {
                buit = it;
                break;
            }
            count_bu++;
        }

        bool found = false;         
        it = buit;
        while (it != auit) {
            if (it == s.end()) it = s.begin();
            if (*it == av) {
                found = true;
                break;
            }
            if (*it == bv) {
                found = true;
                break;
            }
            it++;
        }
        
        if (!found) {
            return s;
        }
        
        // Search for av and then to bv
        bool av_first = false;
        bool bv_first = false;
        list<ListGraph::Node>::iterator avit;
        list<ListGraph::Node>::iterator bvit;
        for (it = s.begin(); it != s.end(); it++) {
            if (*it == av && bv_first == false) {
                av_first = true;
                avit = it;
                break;
            } else if (*it == bv && av_first == false) {
                bv_first = true;
                bvit = it;
                break;
            }
        }
        
        // Create the av_bv list 
        list<ListGraph::Node> av_bv;
        if (av_first) {
            while (*avit != bv) {
                av_bv.push_back(*avit);
                avit++;
                if (avit == s.end()) avit = s.begin();
            }
            av_bv.push_back(bv);
        } else if (bv_first) {
            while (*bvit != av) {
                av_bv.push_back(*bvit);
                bvit++;
                if (bvit == s.end()) bvit = s.begin();
            }
            av_bv.push_back(av);
            av_bv.reverse();
        }
        
        list<ListGraph::Node> au_bu;
        if (count_au < count_bu) {
            while(*auit != bu) {
                au_bu.push_back(*auit);
                auit++;
                if (auit == s.end()) auit = s.begin();
            }
            au_bu.push_back(bu);
        } else {
            while(*buit != au) {
                au_bu.push_back(*buit);
                buit++;
                if (buit == s.end()) buit = s.begin();
            }
            au_bu.push_back(au);
            au_bu.reverse();
        }
        // list<ListGraph::Node> solution;
        au_bu.splice(au_bu.end(), av_bv);
        return au_bu;
        
    } else if (cost_aubv_avbu < cost_ab && cost_aubv_avbu < cost_aubu_avbv) {
        return s;
    } else {
        return s;
    }
    
}

list<ListGraph::Node> Grasp::localSearch(list<ListGraph::Node> solution, ListGraph::EdgeMap<bool> * visited) {
    list<ListGraph::Node> new_solution;
    list<ListGraph::Node>::iterator  solution_it; 
    
    // Find a max weighted edge then change it with the minimun path 
    ListGraph::Edge e = randomEdge(solution);
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

// Execute grasp with the randomSolution method
list<ListGraph::Node> Grasp::execute(double time) {
    Timer t;
    t.restart();
    int selector = 0;
    // While we still have time execute
    while (t.elapsed() < time - 8) {
        unsigned int i = 0;
        ListGraph::EdgeMap<bool> * visited = new ListGraph::EdgeMap<bool>(*graph);
        initVisitedMap(visited);
        list<ListGraph::Node> solution = randomSolution();
        double value = checkBestSolution(solution);
        while (i < num_terminals) {
            double new_value;
            if (num_terminals/node_size == 1) {
                new_value = checkBestSolution(twoopt(solution));
            } else {
                new_value = checkBestSolution(localSearch(solution,visited));
            }
            if (value == new_value) {
                break;
            } else {
                value = new_value;
            }
            i++;
        }
        delete visited;
        selector = (selector + 1) % 3;
    }
    return best_solution;
}

// Execute Grasp with the greed random heuristic
list<ListGraph::Node> Grasp::executeWithGreed(double time) {
    Timer t;
    t.restart();
    
    // While we still have time execute
    while (t.elapsed() < time - 8) {
        unsigned int i = 0;
        ListGraph::EdgeMap<bool> * visited = new ListGraph::EdgeMap<bool>(*graph);
        initVisitedMap(visited);
        list<ListGraph::Node> solution = greedRandomSolution();
        double value = checkBestSolution(solution);
        while (i < num_terminals) {
            double new_value;
            if (num_terminals/node_size == 1) {
                new_value = checkBestSolution(twoopt(solution));
            } else {
                new_value = checkBestSolution(localSearch(solution,visited));
            }
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

// Aux function to init Visited map
void Grasp::initVisitedMap(ListGraph::EdgeMap<bool> * visited) {
    ListGraph::EdgeIt * e = new ListGraph::EdgeIt(*graph);
    for (; *e != INVALID; ++(*e)) {
        (*visited)[*e] = false;
    }
    delete e;
}

// Execute Grasp with the random heuristic with term nodes
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
            double new_value;
            if (num_terminals/node_size == 1) {
                new_value = checkBestSolution(twoopt(solution));
            } else {
                new_value = checkBestSolution(localSearch(solution,visited));
            }
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

// Execute Grasp with Noise and Greed heuristic
list<ListGraph::Node> Grasp::executeWithNoiseAndGreed(double time) {
    Timer t;
    t.restart(); 
    
    // While we still have time
    while(t.elapsed() < time - 8) {
        unsigned int i = 0, count = 0;
        ListGraph::EdgeMap<bool> * visited = new ListGraph::EdgeMap<bool>(*graph);
        initVisitedMap(visited);
        list<ListGraph::Node> solution;
        if (count == 0) {
            solution = randomSolutionWithNoise();
        } else {
            solution = greedRandomSolution();
        }

        double value = checkBestSolution(solution);
        while (i < num_terminals) {
            double new_value;
            if (num_terminals/node_size == 1) {
                new_value = checkBestSolution(twoopt(solution));
            } else {
                new_value = checkBestSolution(localSearch(solution,visited));
            }
            if (value == new_value) {
                break;
            } else {
                value = new_value;
            }
            i++;
        }
        delete visited;
        count = (count + 1) % 2;
    }
    return best_solution;
}

// Store the best solution
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

// Calculate the best value 
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
