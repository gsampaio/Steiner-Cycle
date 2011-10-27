/******************************************************************************
 * SteinerCycleSolver.hpp: Interface for Steiner Cycle Solver class.
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

#ifndef STEINERCYCLESOLVER_HPP
#define STEINERCYCLESOLVER_HPP

#include <lemon/list_graph.h>
using namespace lemon;

/**
 * \brief Steiner Cycle Solver description.
 * \author Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 * \date 2011
 *
 * This class contains the description a generic Steiner Cycle Solver.
 */
class SteinerCycleSolver {
    public:
        /** \name Enumeration Definitions */
        /// Indica o status pos-execução dos algoritmos.
        enum ResultType {
            /// Encontrou uma solução uma solução exata dentro do tempo limite de
            /// tempomax. Isto é apenas para os algoritmos exatos.
            EXACT_SOLUTION = 0,

            /// Encontrou uma solução viável (não necessariamente ótima) dentro do
            /// tempo limite de tempomax. Isto é apenas para os algoritmos exatos.
            EXACT_VIABLE_SOLUTION = 1,

            /// Não encontrou solução dentro do tempo máximo pelo algoritmo exato.
            EXACT_NO_SOLUTION = -1,

            /// Encontrou uma solução viável pela heurística rápida.
            FAST_HEURISTIC_SOLUTION = 2,

            /// Não encontrou solução dentro do tempo máximo pela heurística rápida.
            FAST_HEURISTIC_NO_SOLUTION = -2,

            /// Encontrou uma solução viável pela metaheurística.
            METAHEURISTIC_SOLUTION = 3,

            /// Não encontrou solução dentro do tempo máximo pela metaheurística.
            METAHEURISTIC_NO_SOLUTION = -3
        };

        /// Indica o status de uma possível solução
        enum SolutionStatus {
            /// Se é uma solução válida
            OK = 0,

            /// Se não forma um ciclo ou ele é incorreto.
            INCORRECT_CYCLE,

            /// Se o valor fornecido não está de acordo com o valor do ciclo.
            INCORRECT_VALUE,

            /// Se o ciclo não cobriu todos terminais.
            MISSING_TERMINALS
        };
        //@}

    public:
        /** \name Constructors and Destructor*/
        //@{
        /// Default constructor.
        SteinerCycleSolver();

        /// Destructor.
        virtual ~SteinerCycleSolver() {}
        //@}

        /** \name Main Methods */
        //@{
        /** \brief Run the optimization using an exact algorithm or a metaheuristic.
         * \param max_time to execute in seconds.
         * \return a constant according to \ref ResultType.
         */
        virtual ResultType solve(const double max_time) = 0;

        /** \brief Run the optimization using a fast (polinomial) heuristic.
         * \param max_time to execute in seconds.
         * \return a constant according to \ref ResultType.
         */
        virtual ResultType solveFast(const double max_time) = 0;
        //@}

        /** \name Support Methods */
        //@{
        /** \brief Carrega uma instância.
         * \param filename nome do arquivoque contem a instância.
         */
        virtual bool loadInstance(const char* filename);

        /// Mostra a instância carregada na tela no formato do Lemon
        virtual void displayInstance();

        /** \brief Check if solution is valid.
         *
         * \param solution lista de vértices ordenados no ciclo.
         * \param value valor da solução.
         * \return a solution status according to \ref SolutionStatus.
         */
        SolutionStatus checkSolution(const std::list<ListGraph::Node> &solution,
                                     const double value) const;

        /** \brief Display the problem data. */
        void displaySolution(const std::list<ListGraph::Node> &solution) const;

        /** \brief Look for terminals with degree one.
         * \return true if a terminal leaf was found.
         */
        virtual bool haveTerminalLeaves() const;
        //@}

    public:
        /** \name Data members */
        //@{
        /// Grafo no qual queremos encontrar o ciclo.
        static ListGraph graph;

        /// Peso das arestas.
        static ListGraph::EdgeMap<double> length;

        /// Número de terminais.
        static unsigned num_terminals;

        /// Mapeamento dos vértices terminais.
        static ListGraph::NodeMap<bool> terminal;

        /** \brief Melhor solução encontrada.
         *
         * Uma solução é uma lista de vértices ordenados de tal maneira que
         * formem um ciclo.
         */
        std::list<ListGraph::Node> best_solution;

        /// Valor da melhor solução.
        double solution_value;

        /** \brief Limitante inferior.
         *
         * Apenas para algoritmos exatos que não encontraram
         * solução ótima, volta com o limitante inferior que delimita solução
         * ótima. Caso não tenha sido encontrado nenhum limitante, volta com
         * valor \ref numeric_limits< double >::min().
         */
        double lb;

        /** \brief Limitante superior.
         *
         * Volta com o custo do circuito encontrado. Caso
         * não tenha sido encontrado com nenhum circuito, volta com o valor
         * \ref numeric_limits< double >::max()
         */
        double ub;
        //@}
};

#endif  //STEINERCYCLESOLVER_HPP
