/******************************************************************************
 * RA073177.hpp: Interface for My Steiner Cycle Solver class.
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

#ifndef RA073177_HPP
#define RA073177_HPP

// INCLUS�O OBRIGATORIA PARA HERAN�A DE SUA CLASS
#include "../steiner_cycle_solver.hpp"

// INCLUS�ES ESPECIFICAS DE SEU C�DIGO
#include "GRASP.hpp"
#include "Greedy.hpp"

/**
 * \brief My Steiner Cycle Solver class.
 * \author Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 * \date 2011
 *
 * This class contains the description a Steiner Cycle Solver.
 */
class RA073177: public SteinerCycleSolver, GraspDelegate {
    public:
        /** \name Constructor and Destructor */
        //@{
        RA073177();
        ~RA073177();
        //@}

        /** \name Inherent Methods from SteinerCycleSolver*/
        //@{
        ResultType solve(const double max_time);

        ResultType solveFast(const double max_time);

        virtual bool loadInstance(const char* filename);
        //@}
        
        /** \name Inherent Methods from GraspDelegate  */
        //@{
        virtual double didRecivedSolution(list<ListGraph::Node> solution);
        //}@

        /** \name My methods */
        //@{
        /** \brief Este � um algoritmo guloso, baseado na capacidade da
         * minha gordice por procurar coisas gostosas dentro da geladeira
         * (terminais) passando por coisas n�o t�o gostosas (v�rtices de Steiner).
         */

        list<ListGraph::Node> randomSolution();
        //@}

        /**
        * Function that given an solution calculate the value of it
        */
        double solutionValue(list<ListGraph::Node> solution);
        
        ListGraph::Edge maxWeightedEdge(list<ListGraph::Node> solution);
        list<ListGraph::Node> findBetterPath(list<ListGraph::Node> s, ListGraph::Edge e);
        list<ListGraph::Node> localSearch(list<ListGraph::Node> solution);
        double checkBestSolution(list<ListGraph::Node> solution);
        void grasp();
    protected:
        /** \name Meus par�metros muito doidos*/
        //{@
        // Ok, ok... isso � meio POG, mas � um jeito de contornar a limita��o
        // de n�o poder iniciar constantes no cabe�alho. Usem somente aqui, ok?

        #define TAXA_ALEVINOS       0.7;  ///< Controla a taxa de eclos�o de alevinos
        #define MU_GAMA_ALPHA_ZETA  0.3;  ///< Controla a gerinic�ncia.
        //@}

        /** \name Minhas vari�veis */
        //{@
        //@}
};

#endif  //RA073177_HPP
