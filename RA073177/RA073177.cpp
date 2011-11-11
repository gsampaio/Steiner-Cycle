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

// N�O SE ESQUE�A DE CHAMAR O CONSTRUTOR DA CLASSE BASE EM PRIMEIRO LUGAR
RA073177::RA073177(): SteinerCycleSolver(), grasp(){}

//-------------------------[ Default Destructor ]----------------------------//

RA073177::~RA073177() {}

//-----------------------------[ Set Instance ]------------------------------//

// Se for sobrecarregar esta fun��o, tome cuidado de limpar o grafo antigo.
// Recomendo chamar o m�todo da classe base primeiro.
bool RA073177::loadInstance(const char* filename) {
    return SteinerCycleSolver::loadInstance(filename);
}

//--------------------------------[ Solve ]----------------------------------//

RA073177::ResultType RA073177::solve(const double max_time) {
    cout << "\n--> Resolvendo por um super-ultra-mega-hiper algoritmo doidao em "
         << max_time << " segundos."
         << endl;

    return RA073177::EXACT_NO_SOLUTION;
}

//--------------------------------[ Solve ]----------------------------------//

RA073177::ResultType RA073177::solveFast(const double max_time) {
    cout << "\n--> Resolvendo mega rapido e magico algoritmo em "
         << max_time << " segundos."
         << endl;

    return RA073177::FAST_HEURISTIC_NO_SOLUTION;
}

//------------------------------[ chutaCiclo ]-------------------------------//
void RA073177::chutaCiclo() {

	// First we instantiate an vector with all the terminal nodes
	std::list<ListGraph::Node> tnode;
	for (ListGraph::NodeIt v(graph); v!= INVALID; ++v) {
    	if (terminal[v]) {
			tnode.push_front(v);
		}		
	}

	std::list<ListGraph::Node> sol;
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
}
