/******************************************************************************
 * main.cpp: call optimization algorithms.
 *
 * Author: Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 *
 * (c) Copyright 2011 Institute of Computing, University of Campinas.
 *     All Rights Reserved.
 *
 *  Created on : Oct 01, 2011 by andrade
 *  Last update: Oct 03, 2011 by andrade
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
#include "RA073177/RA073177.hpp"

//-------------------------------[ Main ]------------------------------------//

int main(int argc, char* argv[]) {
    SteinerCycleSolver* solver = new RA073177();
    Timer timer;

    //---------------------- Carrega uma instancia ----------------------//

    if(!solver->loadInstance(argv[1]))
        return -1;

    solver->displayInstance();

    //---------------------- Exemplo de otimização ----------------------//

    timer.restart();
    solver->solve(12.4);
    cout << "No solve(), gastei " << timer.elapsed() << endl;

    timer.restart();
    solver->solveFast(31);
    cout << "No solveFast(), gastei " << timer.elapsed() << endl;

    //------------------ Testando a uma solução --------------------//

    switch(solver->checkSolution(solver->best_solution, 500.0)) {
    case SteinerCycleSolver::INCORRECT_CYCLE:
        cout << "\n\n++ Ciclo incorreto" << endl;
        break;

    case SteinerCycleSolver::INCORRECT_VALUE:
        cout << "\n\n++ valor do ciclo incorreto" << endl;
        break;

    case SteinerCycleSolver::MISSING_TERMINALS:
        cout << "\n\n++ Ciclo nao cobriu todos terminais" << endl;
        break;

    default:
        cout << "\n\n++ solucao correta" << endl;
    }

    //--------------------- Exemplo de saída ---------------------//
    solver->displaySolution(solver->best_solution);

    char result = 'Y';

    // Formata a saida e imprime os resultados
    cout << setiosflags(ios::fixed) << setprecision(2)
         << "\nSolution & Time & Best Solution Value\n"
         << result << " & "
         << timer.elapsed() << " & "
         << (result != 'N'? solver->solution_value : 0.00);

    // NÃO PULE LINHA NO FINAL E NÃO USE ENDL. USE FLUSH() AO INVES.
    cout.flush();

    delete solver;
    return 0;
}
