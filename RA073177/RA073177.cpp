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
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <list>
#include <vector>
#include <cmath>
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
    // Try to open the file
    ifstream file(filename, ios::in);

    // Try to read and parse the file
    try{
        if(!file) {
            throw std::runtime_error("Error on open the file.");
        }
        else {
            file.clear();
            file.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);
        }

        // Some local variables.
        unsigned num_nodes = 0;
        unsigned num_edges = 0;
        unsigned u = 0;
        unsigned v = 0;
        unsigned i = 0;
        double cost = 0.0;

        // Get the first line.
        file >> num_nodes >> num_edges >> num_terminals;
        
        // Set internal var value
        node_size = num_nodes;
        
        if(file.fail())
            throw std::runtime_error("Something is wrong on loading the size of the graph!");

        if(num_nodes < num_terminals)
            throw std::runtime_error("Number of terminal is greater than number of nodes");

        // Create all nodes first.
        vector<ListGraph::Node> nodes(num_nodes);
        for(i = 0; i < num_nodes; ++i)
            nodes[i] = graph.addNode();

        // Create edges if any.
        for(i = 0; i < num_edges; ++i) {
            file >> u >> v >> cost;
            if(file.fail() || file.eof())
                throw std::runtime_error("Something is wrong on loading edges!");

            length[graph.addEdge(nodes[u], nodes[v])] = cost;
        }

        if(i != num_edges)
            throw std::runtime_error("Number of loaded edges is incorrect!");

        // Load terminals.
        for(i = 0; i < num_terminals; ++i) {
            file >> u;
            if(file.fail() || file.eof())
                throw std::runtime_error("Something is wrong on loading terminals!");

            terminal[nodes[u]] = true;
        }

        if(i != num_terminals)
            throw std::runtime_error("Number of loaded terminals is incorrect!");
    }
    catch(exception& e) {
        cerr << "\n**********************************************************\n"
             << "Error on " <<  __PRETTY_FUNCTION__
             <<  ": line " << __LINE__
             << "\nData file cannot be loaded: " << filename
             << "\nError: " << e.what()
             << "\n**********************************************************"
             << endl;

        file.close();
        return false;
    }

    return true;
}

//--------------------------------[ Solve ]----------------------------------//

RA073177::ResultType RA073177::solve(const double max_time) {
    // First we get the number max threads
    int nthreads = omp_get_max_threads();

    // We set up 4 instances
    if (nthreads > 2) {
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
                        Grasp grasp1((GraspDelegate*)this,g1,terminal,length,num_terminals, node_size);            
                        //list<ListGraph::Node> sol1 = grasp1.executeWithNoise(max_time);
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
                        Grasp grasp2((GraspDelegate*)this,g2,terminal,length,num_terminals, node_size);            
                        //list<ListGraph::Node> sol2 = grasp2.executeWithNoise(max_time);
                        list<ListGraph::Node> sol2 = grasp2.executeWithGreed(max_time);
                        #pragma omp critical 
                        {
                            checkBestSolution(sol2);                        
                        }            
                    }
                
                    #pragma omp section 
                    {
                        ListGraph g3;
                        ListGraph::NodeMap<bool> t3(g3);
                        ListGraph::EdgeMap<double> l3(g3);
                        ListGraph::NodeMap<ListGraph::Node> nm3(graph);
                        ListGraph::EdgeMap<ListGraph::Edge> em3(graph);
                        graphCopy(graph, g3).nodeRef(nm3).edgeRef(em3).nodeMap(terminal, t3).edgeMap(length, l3).run();
                        Grasp grasp3((GraspDelegate*)this,g3,terminal,length,num_terminals, node_size);            
                        list<ListGraph::Node> sol3 = grasp3.execute(max_time);
                        #pragma omp critical 
                        {
                            checkBestSolution(sol3);                        
                        }              
                    }
                    #pragma omp section 
                    {
                        ListGraph g4;
                        ListGraph::NodeMap<bool> t4(g4);
                        ListGraph::EdgeMap<double> l4(g4);
                        ListGraph::NodeMap<ListGraph::Node> nm4(graph);
                        ListGraph::EdgeMap<ListGraph::Edge> em4(graph);
                        graphCopy(graph, g4).nodeRef(nm4).edgeRef(em4).nodeMap(terminal, t4).edgeMap(length, l4).run();
                        Grasp grasp4((GraspDelegate*)this,g4,terminal,length,num_terminals, node_size);            
                        list<ListGraph::Node> sol4 = grasp4.execute(max_time);
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
                    Grasp grasp1((GraspDelegate*)this,g1,terminal,length,num_terminals, node_size);            
                    list<ListGraph::Node> sol1 = grasp1.execute(max_time);
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
                    Grasp grasp2((GraspDelegate*)this,g2,terminal,length,num_terminals, node_size);
                    list<ListGraph::Node> sol2 = grasp2.executeWithNoiseAndGreed(max_time);
                    #pragma omp critical 
                    {
                        checkBestSolution(sol2);                        
                    }
                }
            }
        }
    } else {
        // For one instance
        ListGraph g1;
        ListGraph::NodeMap<bool> t1(g1);
        ListGraph::EdgeMap<double> l1(g1);
        ListGraph::NodeMap<ListGraph::Node> nm1(graph);
        ListGraph::EdgeMap<ListGraph::Edge> em1(graph);
        Grasp grasp1((GraspDelegate*)this,graph,terminal,length,num_terminals, node_size);
        list<ListGraph::Node> sol2 = grasp1.execute(max_time);
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

// Calculate the value of the solution
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

// Store the best solution
double RA073177::checkBestSolution(list<ListGraph::Node> solution) {
    double sol_value = solutionValue(solution);
    if (best_solution.size() > 0) {
        // Check for the best solution
        double best_solution_value = solutionValue(best_solution);
        if (best_solution_value > sol_value) {
            best_solution = solution;
            solution_value = sol_value;
        }
    } else {
        // Case there is no best solution yet we just set it
        best_solution = solution;
        solution_value = sol_value;
    }
    return sol_value;
}