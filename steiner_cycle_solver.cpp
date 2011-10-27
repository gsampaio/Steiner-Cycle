/******************************************************************************
 * SteinerCycleSolver.cpp: Implementation for Steiner Cycle Solver class.
 *
 * Author: Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 *
 * (c) Copyright 2011 Institute of Computing, University of Campinas.
 *     All Rights Reserved.
 *
 *  Created on : Oct 01, 2011 by andrade
 *  Last update: Oct 07, 2011 by andrade
 *
 * This software is licensed under the Common Public License. Please see
 * accompanying file for terms.
 *****************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <list>
#include <vector>
#include <cmath>
#include <lemon/lgf_writer.h>
using namespace std;

#include "steiner_cycle_solver.hpp"

//--------------------------[ Static attributes ]----------------------------//

ListGraph SteinerCycleSolver::graph;
ListGraph::EdgeMap<double> SteinerCycleSolver::length(SteinerCycleSolver::graph);
unsigned SteinerCycleSolver::num_terminals = 0;
ListGraph::NodeMap<bool> SteinerCycleSolver::terminal(SteinerCycleSolver::graph);

//-------------------------[ Default Construtor ]----------------------------//

SteinerCycleSolver::SteinerCycleSolver():
    best_solution(),
    solution_value(numeric_limits< double >::max()),
    lb(numeric_limits< double >::min()),
    ub(numeric_limits< double >::max())
    {}

//----------------------------[ Check Solution ]-----------------------------//

// Se alguém encontrar uma maneira melhor de fazer isso, usando
// ferramentas do lemon, implemente e deixe claro no relatório
// para que seja considerado na hora da nota
SteinerCycleSolver::SolutionStatus SteinerCycleSolver::checkSolution(
                                    const std::list<ListGraph::Node> &solution,
                                    const double value) const {

    list<ListGraph::Node>::const_iterator u, v;
    double value_prime = 0.0;
    unsigned term_num = 0;

    v = solution.begin();

    for(unsigned i = 0; i < solution.size(); ++i) {
        u = v++;

        // Fecha o ciclo
        if(v == solution.end())
            v = solution.begin();

        // Procura por aresta válida
        ListGraph::IncEdgeIt e(graph, *u);
        for(; e != INVALID; ++e) {
            if((graph.u(e) == *v) || (graph.v(e) == *v))
                break;
        }

        // Se for inválido, é pq não achou a aresta.
        if(e == INVALID)
            return INCORRECT_CYCLE;

        // Soma valor
        value_prime += length[e];

        // Conta os terminais
        if(terminal[*u])
            ++term_num;
    }

    if(abs(value_prime - value) > 0.0001)
        return INCORRECT_VALUE;

    if(term_num != this->num_terminals)
        return MISSING_TERMINALS;

    return OK;
}

//------------------------[ Have Terminal Leaves ]---------------------------//

bool SteinerCycleSolver::haveTerminalLeaves() const {
    bool leafs = false;

    // Looking for terminal leafs.
    for(ListGraph::NodeIt v(graph); v != INVALID; ++v)
        if(terminal[v]) {
            if(countIncEdges(graph, v) < 2)
                leafs = true;
                break;
            }

    return leafs;
}

//---------------------------[ Display Instance ]----------------------------//

void SteinerCycleSolver::displayInstance() {
    graphWriter(graph)
        .nodeMap("Terminal", terminal)
        .edgeMap("Cost", length)
        .run();
}

//---------------------------[ Display Solution ]----------------------------//

void SteinerCycleSolver::displaySolution(const std::list<ListGraph::Node> &solution) const {
    cout << "\n\nBest cycle\n";

    for(list<ListGraph::Node>::const_iterator it = solution.begin();
        it != solution.end(); ++it)
        cout << graph.id(*it) << " ";

    cout << "\nendcycle" << endl;
}

//---------------------------[ Display Solution ]----------------------------//

bool SteinerCycleSolver::loadInstance(const char* filename) {
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
