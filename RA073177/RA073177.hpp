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

// INCLUSÃO OBRIGATORIA PARA HERANÇA DE SUA CLASS
#include "../steiner_cycle_solver.hpp"

// INCLUSÕES ESPECIFICAS DE SEU CÓDIGO
#include "GRASP.hpp"

/**
 * \brief My Steiner Cycle Solver class.
 * \author Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 * \date 2011
 *
 * This class contains the description a Steiner Cycle Solver.
 */
class RA073177: public SteinerCycleSolver {
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

        /** \name My methods */
        //@{
        /** \brief Este é um algoritmo guloso, baseado na capacidade da
         * minha gordice por procurar coisas gostosas dentro da geladeira
         * (terminais) passando por coisas não tão gostosas (vértices de Steiner).
         */
        void chutaCiclo();
        //@}

    protected:
        /** \name Meus parâmetros muito doidos*/
        //{@
        // Ok, ok... isso é meio POG, mas é um jeito de contornar a limitação
        // de não poder iniciar constantes no cabeçalho. Usem somente aqui, ok?

        #define TAXA_ALEVINOS       0.7;  ///< Controla a taxa de eclosão de alevinos
        #define MU_GAMA_ALPHA_ZETA  0.3;  ///< Controla a gerinicência.
        //@}

        /** \name Minhas variáveis */
        //{@
        Grasp grasp;
        //@}
};

#endif  //RA073177_HPP
