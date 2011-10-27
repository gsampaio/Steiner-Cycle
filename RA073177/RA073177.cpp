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
RA073177::RA073177(): SteinerCycleSolver(),
    ze(), grasp(){}

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
    // Primeiro, fazemos isso e aquilo usando tal ideia
    //...
    //...
    // Depois, pegamos tal coisa e invertemos usando um 2OPT, etc e tal...
    //...
    //...

    //ze.roda();
    if(grasp == NULL) {
        prinf("testessssssssss");
    }
    grasp.execute();
   // cout << "\n:-P" << endl;

    //cout << "\nnodes: ";
    //for(ListGraph::NodeIt v(graph); v != INVALID; ++v)
      //   cout << graph.id(v) << ": " << terminal[v] << " | ";

    //cout << "\nedges: ";
   // for(ListGraph::EdgeIt e(graph); e != INVALID; ++e)
     //    cout << "(" << graph.id(graph.u(e))
       //       << "," << graph.id(graph.v(e))
         //     << "): " << length[e]
           //   << " | ";

    cout << endl;
}
