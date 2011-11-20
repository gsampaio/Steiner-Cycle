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
 
#include <omp.h> 
#include <iostream>
using namespace std;

#include <lemon/adaptors.h>
#include <lemon/dijkstra.h>
#include <lemon/list_graph.h>
using namespace lemon;

#include <vector>

#include <stdio.h>

#include "RA073177.hpp"
#include "../mtrand.hpp"

//-------------------------[ Default Construtor ]----------------------------//

// NÃO SE ESQUEÇA DE CHAMAR O CONSTRUTOR DA CLASSE BASE EM PRIMEIRO LUGAR
RA073177::RA073177(): SteinerCycleSolver(), GraspDelegate(){}
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
    //  //cout << "\n--> Valor da minha solução "
    //      << solutionValue(solution) << " com aresta maxima de peso "
    //      << length[maxWeighted]
    //      << endl;
    // 
    //  findBetterPath(solution, maxWeighted);
    // GreedyHeuristic greedy(graph,terminal,length,num_terminals);
    //best_solution = greedy.execute();
    
    int nthreads = omp_get_max_threads();
    //cout << "NUMERO DE THREADS: " << nthreads << endl;
    omp_set_num_threads(2);
    
    // #pragma omp parallel for
    // for (int i = 0; i < omp_get_max_threads(); i++) {
    //     Grasp grasp((GraspDelegate*)this,graph,terminal,length,num_terminals);
    //     if (i % 2 == 0) {
    //         grasp.executeWithNoise(max_time);                    
    //     } else {
    //         grasp.execute(max_time);                                
    //     }
    // }    
    
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section 
            {
                //cout << "comecou grasp1" << endl;;
                Grasp grasp1((GraspDelegate*)this,graph,terminal,length,num_terminals);
                grasp1.executeWithNoise(max_time);
                //cout << "acabou grasp1" << endl;;
            }
            #pragma omp section 
            {
                //cout << "comecou grasp2" << endl;;
                Grasp grasp2((GraspDelegate*)this,graph,terminal,length,num_terminals);
                grasp2.execute(max_time);       
                //cout << "acabou grasp2" << endl;         
            }
        }
        //cout << "ACABOU O METODO";
    }
    return RA073177::METAHEURISTIC_SOLUTION;
}

//--------------------------------[ Solve ]----------------------------------//

RA073177::ResultType RA073177::solveFast(const double max_time) {
    // //cout << "\n--> Resolvendo mega rapido e magico algoritmo em "
    //      << max_time << " segundos."
    //      << endl;
    return RA073177::FAST_HEURISTIC_NO_SOLUTION;
}


double RA073177::didRecivedSolution(list<ListGraph::Node> solution) {
    return checkBestSolution(solution);
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

double RA073177::checkBestSolution(list<ListGraph::Node> solution) {

    // //cout << "##### checkBestSolution::BEGIN #####" << endl;
    // //cout << "\tBEST_SOLUTION_SIZE = " << best_solution.size() << endl;
    double sol_value = solutionValue(solution);
    if (best_solution.size() > 0) {
        double best_solution_value = solutionValue(best_solution);
        // //cout << "\tBEST_SOL_VAL: " << best_solution_value << endl;
        // //cout << "\tSOL_VALUE___: " << sol_value << endl;
        if (best_solution_value > sol_value) {
            best_solution = solution;
            solution_value = sol_value;
        }
    } else {
        best_solution = solution;
        solution_value = sol_value;
    }
    return sol_value;
}