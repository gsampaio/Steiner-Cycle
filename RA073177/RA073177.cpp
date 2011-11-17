/******************************************************************************
 * RA073177.cpp: Implementation for My Steiner Cycle Solver class.
 *
 * Author: Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 *
 * (c) Copyright 2011 Institute of Computing, University of Campinas.
 *     All Rights Reserved.
 *
 *  Created on : Oct 01, 2011 by andrade
 *  Last update: Oct 02, 2011 by andrade
 *
 * This software is licensed under the Common Public License. Please see
 * accompanying file for terms.
 *****************************************************************************/

#include <iostream>
using namespace std;

#include <lemon/adaptors.h>
#include <lemon/dijkstra.h>
#include <lemon/list_graph.h>
using namespace lemon;

#include "RA073177.hpp"
#include "../mtrand.hpp"

//-------------------------[ Default Construtor ]----------------------------//

// NÃO SE ESQUEÇA DE CHAMAR O CONSTRUTOR DA CLASSE BASE EM PRIMEIRO LUGAR
RA073177::RA073177(): SteinerCycleSolver(){}

//-------------------------[ Default Destructor ]----------------------------//

RA073177::~RA073177() {}

//-----------------------------[ Set Instance ]------------------------------//

// Se for sobrecarregar esta função, tome cuidado de limpar o grafo antigo.
// Recomendo chamar o método da classe base primeiro.
bool RA073177::loadInstance(const char* filename) {
    return SteinerCycleSolver::loadInstance(filename);
}

//--------------------------------[ Solve ]----------------------------------//

RA073177::ResultType RA073177::solve(const double max_time) {
    // list<ListGraph::Node> solution = randomSolution();
    // ListGraph::Edge maxWeighted = maxWeightedEdge(solution);
    //  cout << "\n--> Valor da minha solução "
    //      << solutionValue(solution) << " com aresta maxima de peso "
    //      << length[maxWeighted]
    //      << endl;
    // 
    //  findBetterPath(solution, maxWeighted);
    grasp();
    return RA073177::EXACT_NO_SOLUTION;
}

//--------------------------------[ Solve ]----------------------------------//

RA073177::ResultType RA073177::solveFast(const double max_time) {
    // cout << "\n--> Resolvendo mega rapido e magico algoritmo em "
    //      << max_time << " segundos."
    //      << endl;

    return RA073177::FAST_HEURISTIC_NO_SOLUTION;
}

//------------------------------[ randomSolution ]-------------------------------//
list<ListGraph::Node> RA073177::randomSolution() {

	// First we instantiate an vector with all the terminal nodes
	std::list<ListGraph::Node> tnode;
	for (ListGraph::NodeIt v(graph); v!= INVALID; ++v) {
    	if (terminal[v]) {
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
		cout << graph.id(*iter) << " ";
	}
    cout << endl;
    return sol;
}

double RA073177::solutionValue(list<ListGraph::Node> solution) {
    list<ListGraph::Node>::const_iterator u, v;
    double value = 0.0;
    v = solution.begin();
    
    for (unsigned int i =0; i < solution.size(); ++i) {
        u = v++;
        
        if (v == solution.end()) v = solution.begin();
        
        // Search for the valid edge
        ListGraph::IncEdgeIt e(graph, *u);
        for (;e != INVALID; ++e) {
            if((graph.u(e) == *v) || (graph.v(e) == *v)) {
                break;
            } 
        }
        
        if (e == INVALID) return -1;
        
        value += length[e];
    }
    return value;
}

ListGraph::Edge RA073177::maxWeightedEdge(list<ListGraph::Node> solution) {
    list<ListGraph::Node>::const_iterator u, v;
    double max = 0.0;
    v = solution.begin();
    ListGraph::Edge maxEdge;
    for (unsigned int i =0; i < solution.size(); ++i) {
        u = v++;
        
        if (v == solution.end()) v = solution.begin();
        
        // Search for the valid edge
        ListGraph::IncEdgeIt e(graph, *u);
        for (;e != INVALID; ++e) {
            if((graph.u(e) == *v) || (graph.v(e) == *v)) {
                break;
            } 
        }
        
        if (length[e] > max) {
            max = length[e];
            maxEdge = e;
        }
    }
    
    return maxEdge;
}

list<ListGraph::Node> RA073177::findBetterPath(list<ListGraph::Node> s, ListGraph::Edge e) {
    
    typedef const ListGraph::EdgeMap<double> LengthMap;
    typedef Dijkstra<const SubGraph<ListGraph>, LengthMap> ShortestPath;
    
    ListGraph::NodeMap<bool> node_filter(graph);
    ListGraph::EdgeMap<bool> edge_filter(graph);
    list<ListGraph::Node> nodesToRemove;
    
    // Create the node_filter
    for (ListGraph::NodeIt v(graph); v != INVALID; ++v) {
        cout << "Nó atual " << graph.id(v) << endl;
        if((graph.u(e) != v) && (graph.v(e) != v)) {
            list<ListGraph::Node>::iterator iter;
            for (iter = s.begin(); iter != s.end(); iter++) {
                cout << "\tatual da solucao = " << graph.id(*iter) <<endl;
                if(graph.id(v) == graph.id(*iter)) {
                    cout << "\t\titem " << graph.id(v) << " removido" << endl;
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
    for (ListGraph::EdgeIt ed(graph); ed != INVALID; ++ed) {
        list<ListGraph::Node>::iterator iter;
        bool value = true;
        for (iter = nodesToRemove.begin(); iter != nodesToRemove.end(); iter++) {
            if ( graph.u(ed) == *iter || graph.v(ed) == *iter) {
                value = false;
            }
        }
        edge_filter[ed] = value;
    }
    
    SubGraph<ListGraph> sg(graph, node_filter, edge_filter);
    ShortestPath d(sg, length);
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

list<ListGraph::Node> RA073177::localSearch(list<ListGraph::Node> solution) {
    list<ListGraph::Node> new_solution;
    list<ListGraph::Node>::iterator solution_it; 
    
    ListGraph::Edge e = maxWeightedEdge(solution);
    list<ListGraph::Node> path = findBetterPath(solution, e);
    for (solution_it = solution.begin(); solution_it != solution.end(); solution_it++) {
        if (graph.u(e) == *solution_it) {
            list<ListGraph::Node>::iterator path_it; 
            for (path_it = path.begin(); path_it != path.end(); path_it++) {
                new_solution.push_back(*path_it);
            }
        } else if (graph.v(e) == *solution_it) {
            continue;
        } else {
            new_solution.push_back(*solution_it);
        }
    }
    
    cout << "NEW SOLUTION: ";
    list<ListGraph::Node>::iterator it;
    for (it = new_solution.begin(); it != new_solution.end(); it++) {
        cout << graph.id(*it) << " ";
    }
    cout << endl;
    
    return new_solution;
}

bool RA073177::checkBestSolution(list<ListGraph::Node> solution) {
    cout << "##### checkBestSolution::BEGIN #####" << endl;
    cout << "\tBEST_SOLUTION_SIZE = " << best_solution.size() << endl;
    if (best_solution.size() > 0) {
        double best_solution_value = solutionValue(best_solution);
        double sol_value = solutionValue(solution);
        cout << "\tBEST_SOL_VAL: " << best_solution_value << endl;
        cout << "\tSOL_VALUE___: " << sol_value << endl;
        if (best_solution_value > sol_value) {
            best_solution = solution;
            solution_value = sol_value;
        } else if (best_solution_value == solution_value) {
            return false;
        }
    } else {
        best_solution = solution;
        double sol_value = solutionValue(solution);
        solution_value = sol_value;
    }
    return true;
}

void RA073177::grasp() {
    unsigned int i = 0;
    ListGraph::Edge e;
    list<ListGraph::Node> solution = randomSolution();
    bool troca = checkBestSolution(solution);
    while (troca && i < num_terminals) {
        troca = checkBestSolution(localSearch(solution));
        i++;
    }
}