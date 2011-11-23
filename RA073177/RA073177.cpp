/******************************************************************************
 * RA073177.cpp: Implementation for My Steiner Cycle Solver class.
 *
 * Author: Guilherme Martinez Sampaio <guilhermesampaio@gmail.com>
 *
 * (c) Copyright 2011 Institute of Computing, University of Campinas.
 *     All Rights Reserved.
 *
 *  Created on : Oct 01, 2011 by andrade
 *  Last update: Nov 21, 2011 by gsampaio
 *
 * This software is licensed under the Common Public License. Please see
 * accompanying file for terms.
 *****************************************************************************/
 
#include <omp.h> 
#include <iostream>
using namespace std;

#include <lemon/core.h>
#include <lemon/adaptors.h>
#include <lemon/dijkstra.h>
#include <lemon/list_graph.h>
#include <lemon/lgf_writer.h>
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
    int nthreads = omp_get_max_threads();
    // We set up 4 instances
    if (nthreads > 2) {
        #pragma omp parallel
            {
                #pragma omp sections
                {
                    #pragma omp section 
                    {
                        ListGraph *g1;
                        ListGraph::NodeMap<bool> * t1 = new ListGraph::NodeMap<bool>(*g1);
                        ListGraph::EdgeMap<double> * l1 = new ListGraph::EdgeMap<double>(*g1);
                        ListGraph::NodeMap<ListGraph::Node> * nm1 = new ListGraph::NodeMap<ListGraph::Node>(graph);
                        ListGraph::EdgeMap<ListGraph::Edge> * em1 = new ListGraph::EdgeMap<ListGraph::Edge>(graph);
                        graphCopy(graph, *g1).nodeRef(*nm1).edgeRef(*em1).nodeMap(terminal, *t1).edgeMap(length, *l1).run();
                        Grasp grasp1((GraspDelegate*)this,*g1,terminal,length,num_terminals);            
                        list<ListGraph::Node> sol1 = grasp1.executeWithNoise(max_time);
                        delete g1;
                        delete t1;
                        delete l1;
                        delete nm1;
                        delete em1;
                        #pragma omp critical 
                        {
                            checkBestSolution(sol1);                        
                        }
                    }
                    #pragma omp section 
                    {
                        ListGraph *g2;
                        ListGraph::NodeMap<bool> * t2 = new ListGraph::NodeMap<bool>(*g2);
                        ListGraph::EdgeMap<double> * l2 = new ListGraph::EdgeMap<double>(*g2);
                        ListGraph::NodeMap<ListGraph::Node> * nm2 = new ListGraph::NodeMap<ListGraph::Node>(graph);
                        ListGraph::EdgeMap<ListGraph::Edge> * em2 = new ListGraph::EdgeMap<ListGraph::Edge>(graph);
                        graphCopy(graph, *g2).nodeRef(*nm2).edgeRef(*em2).nodeMap(terminal, *t2).edgeMap(length, *l2).run();
                        Grasp grasp2((GraspDelegate*)this,*g2,terminal,length,num_terminals);            
                        list<ListGraph::Node> sol2 = grasp2.executeWithNoise(max_time);
                        delete g2;
                        delete t2;
                        delete l2;
                        delete nm2;
                        delete em2;
                        #pragma omp critical 
                        {
                            checkBestSolution(sol2);                        
                        }                    
                    }
                
                    #pragma omp section 
                    {
                        ListGraph *g3;
                        ListGraph::NodeMap<bool> * t3 = new ListGraph::NodeMap<bool>(*g3);
                        ListGraph::EdgeMap<double> * l3 = new ListGraph::EdgeMap<double>(*g3);
                        ListGraph::NodeMap<ListGraph::Node> * nm3 = new ListGraph::NodeMap<ListGraph::Node>(graph);
                        ListGraph::EdgeMap<ListGraph::Edge> * em3 = new ListGraph::EdgeMap<ListGraph::Edge>(graph);
                        graphCopy(graph, *g3).nodeRef(*nm3).edgeRef(*em3).nodeMap(terminal, *t3).edgeMap(length, *l3).run();
                        Grasp grasp3((GraspDelegate*)this,*g3,terminal,length,num_terminals);            
                        list<ListGraph::Node> sol3 = grasp3.executeWithNoise(max_time);
                        delete g3;
                        delete t3;
                        delete l3;
                        delete nm3;
                        delete em3;
                        #pragma omp critical 
                        {
                            checkBestSolution(sol3);                        
                        }                    
                    }
                    #pragma omp section 
                    {
                        ListGraph *g4;
                        ListGraph::NodeMap<bool> * t4 = new ListGraph::NodeMap<bool>(*g4);
                        ListGraph::EdgeMap<double> * l4 = new ListGraph::EdgeMap<double>(*g4);
                        ListGraph::NodeMap<ListGraph::Node> * nm4 = new ListGraph::NodeMap<ListGraph::Node>(graph);
                        ListGraph::EdgeMap<ListGraph::Edge> * em4 = new ListGraph::EdgeMap<ListGraph::Edge>(graph);
                        graphCopy(graph, *g4).nodeRef(*nm4).edgeRef(*em4).nodeMap(terminal, *t4).edgeMap(length, *l4).run();
                        Grasp grasp4((GraspDelegate*)this,*g4,terminal,length,num_terminals);            
                        list<ListGraph::Node> sol4 = grasp4.executeWithNoise(max_time);
                        delete g4;
                        delete t4;
                        delete l4;
                        delete nm4;
                        delete em4;
                        #pragma omp critical 
                        {
                            checkBestSolution(sol4);
                        }
                    }
            }
        }
    } else if (nthreads == 2) {
        // we set up 2 instances
        #pragma omp parallel
        {
            #pragma omp sections
            {
                #pragma omp section 
                {   
                    ListGraph g1;
                    ListGraph::NodeMap<bool> t1(g1);
                    ListGraph::EdgeMap<double> l1(g1);
                    ListGraph::NodeMap<ListGraph::Node> nm1(graph);
                    ListGraph::EdgeMap<ListGraph::Edge> em1(graph);
                    graphCopy(graph, g1).nodeRef(nm1).edgeRef(em1).nodeMap(terminal, t1).edgeMap(length, l1).run();
                    Grasp grasp1((GraspDelegate*)this,g1,terminal,length,num_terminals);            
                    list<ListGraph::Node> sol1 = grasp1.executeWithNoise(max_time);
                    #pragma omp critical 
                    {
                        checkBestSolution(sol1);                        
                    }
                }
                #pragma omp section 
                {
                    ListGraph g2;
                    ListGraph::NodeMap<bool> t2(g2);
                    ListGraph::EdgeMap<double> l2(g2);
                    ListGraph::NodeMap<ListGraph::Node> nm2(graph);
                    ListGraph::EdgeMap<ListGraph::Edge> em2(graph);
                    graphCopy(graph, g2).nodeRef(nm2).edgeRef(em2).nodeMap(terminal, t2).edgeMap(length, l2).run();
                    Grasp grasp2((GraspDelegate*)this,g2,terminal,length,num_terminals);
                    list<ListGraph::Node> sol2 = grasp2.execute(max_time);
                    #pragma omp critical 
                    {
                        checkBestSolution(sol2);                        
                    }
                }
            }
        }
    } else {
        ListGraph g1;
        ListGraph::NodeMap<bool> t1(g1);
        ListGraph::EdgeMap<double> l1(g1);
        ListGraph::NodeMap<ListGraph::Node> nm1(graph);
        ListGraph::EdgeMap<ListGraph::Edge> em1(graph);
        Grasp grasp1((GraspDelegate*)this,graph,terminal,length,num_terminals);
        list<ListGraph::Node> sol2 = grasp1.executeWithNoise(max_time);
        checkBestSolution(sol2);
    }

    return RA073177::METAHEURISTIC_SOLUTION;
}

//--------------------------------[ Solve ]----------------------------------//

RA073177::ResultType RA073177::solveFast(const double max_time) {

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
    double sol_value = solutionValue(solution);
    if (best_solution.size() > 0) {
        double best_solution_value = solutionValue(best_solution);
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