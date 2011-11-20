/******************************************************************************
 * main.cpp: call optimization algorithms.
 *
 * Author: Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 *
 * (c) Copyright 2011 Institute of Computing, University of Campinas.
 *     All Rights Reserved.
 *
 *  Created on : Oct 01, 2011 by andrade
 *  Last update: Nov 16, 2011 by andrade
 *
 * This software is licensed under the Common Public License. Please see
 * accompanying file for terms.
 *****************************************************************************/

#include <iostream>
#include <iomanip>
#include <list>
using namespace std;

#include <lemon/list_graph.h>
using namespace lemon;

#include "timer.hpp"
#include "mtrand.hpp"

//TODO: Coloque seu reader aqui.
#include "RA073177/RA073177.hpp"

//-------------------------------[ Main ]------------------------------------//

int main(int argc, char* argv[]) {
    // First read parameters from command line:
    if(argc < 3) {
        cerr << "usage: " << argv[0]
             << " <arquivo-entrada> <tempo-maximo>"
             << "\nonde: "
             << "\n - <arquivo-entrada>: uma instancia do problema"
             << "\n - <tempo-maximo>: tempo maximo que o algoritmo serah executado."
             << "\n\n AMBOS PARAMETROS SAO NECESSARIOS\n"
             << endl;
        return 64;  // BSD usage error code
    }


    //TODO: Instancie sua classe aqui.
    SteinerCycleSolver* solver = new RA073177();
    Timer timer;
    double elapsed_time;

    // Carrega uma instância
    if(!solver->loadInstance(argv[1])) {
        delete solver;
        return -1;
    }

    // Inicia a otimização
    timer.restart();
    SteinerCycleSolver::ResultType result = solver->solve(atof(argv[2]));
    elapsed_time = timer.elapsed();

    // Testa se a soluação retornada é válida
    bool solucao_valida = false;
    switch(solver->checkSolution(solver->best_solution, solver->solution_value)) {
        case SteinerCycleSolver::INCORRECT_CYCLE:
            cout << " & Ciclo incorreto";
            break;

        case SteinerCycleSolver::INCORRECT_VALUE:
            cout << " & Valor do ciclo incorreto";
            break;

        case SteinerCycleSolver::MISSING_TERMINALS:
            cout << " & Ciclo nao cobriu todos terminais";
            break;

        default:
            solucao_valida = true;
    }

    if(solucao_valida) {
        //--------------------- Exemplo de saída ---------------------//
        solver->displaySolution(solver->best_solution);

        char result_symbol;

        switch(result) {
            case SteinerCycleSolver::EXACT_SOLUTION:
                result_symbol = 'O';
                break;

            case SteinerCycleSolver::EXACT_VIABLE_SOLUTION:
            case SteinerCycleSolver::FAST_HEURISTIC_SOLUTION:
            case SteinerCycleSolver::METAHEURISTIC_SOLUTION:
                result_symbol = 'Y';
                break;

            default:
                result_symbol = 'N';
        }

        // Formata a saida e imprime os resultados
        cout << setiosflags(ios::fixed) << setprecision(2)
             << "\nSolution & Time & Best Solution Value\n"
             << result_symbol << " & "
             << elapsed_time << " & "
             << (result_symbol != 'N'? solver->solution_value : 0.00);
    }

    cout.flush();
    delete solver;
    return 0;
}
