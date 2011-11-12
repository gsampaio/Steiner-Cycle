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

#include <lemon/list_graph.h>
using namespace lemon;

#include "RA073177.hpp"
#include "../mtrand.hpp"

//-------------------------[ Default Construtor ]----------------------------//

// NÃO SE ESQUEÇA DE CHAMAR O CONSTRUTOR DA CLASSE BASE EM PRIMEIRO LUGAR
RA073177::RA073177(): SteinerCycleSolver(), grasp(){}

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
    list<ListGraph::Node> solution = randomSolution();
    
     cout << "\n--> Valor da minha solução "
         << solutionValue(solution) << " com aresta maxima de peso "
         << length[maxWeightedEdge(solution)]
         << endl;
    
     return RA073177::EXACT_NO_SOLUTION;
}

//--------------------------------[ Solve ]----------------------------------//

RA073177::ResultType RA073177::solveFast(const double max_time) {
    // cout << "\n--> Resolvendo mega rapido e magico algoritmo em "
    //      << max_time << " segundos."
    //      << endl;

    return RA073177::FAST_HEURISTIC_NO_SOLUTION;
}

//------------------------------[ chutaCiclo ]-------------------------------//
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
