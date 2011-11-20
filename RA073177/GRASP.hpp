#ifndef GRASP_HPP
#define GRASP_HPP

#include <iostream>
using namespace std;

#include <lemon/list_graph.h>
using namespace lemon;

class GraspDelegate {
    public:
        /**\name Interface Methods **/
        //@{
            
        /**\brief Interface para servir de intercambio de dados entre a 
         * classe principale e o grasp com intuito de guardar o melhor valor.
         * @return retorna se o algoritmo deve continuar executando
         */
        virtual double didRecivedSolution(list<ListGraph::Node> solution) = 0;
        //@}
};


class Grasp {
    public:
        /** \name Constructor and Destructor */
        //@{
        Grasp(GraspDelegate* d,
              ListGraph &g, 
              ListGraph::NodeMap<bool> &t, 
              ListGraph::EdgeMap<double> &l,
              unsigned int nt);
        //@}
        
        /** \name Metodos  */
        //@{
        /** \brief Funcao de gerar solucoes aleatorias, seu funcionamento eh baseado em
         *   pegar todos os verticies terminais de forma aleatoria e fazer ligar
         *  um aos outros formando o ciclo da solução 
         */
        list<ListGraph::Node> randomSolution();
        //}@
        list<ListGraph::Node> randomSolutionWithNoise();

        ListGraph::Edge maxWeightedEdge(list<ListGraph::Node> solution);
        list<ListGraph::Node> findBetterPath(list<ListGraph::Node> s, ListGraph::Edge e);
        list<ListGraph::Node> localSearch(list<ListGraph::Node> solution);
        double sendSolution(list<ListGraph::Node> solution);
        void execute(double time);
        void executeWithNoise(double time);
        
    protected:
        GraspDelegate * delegate;
        ListGraph * graph;
        ListGraph::NodeMap<bool> * terminal;
        ListGraph::EdgeMap<double> * length;
        unsigned int num_terminals;
        list<ListGraph::Edge> removed;
        
};


#endif  //GRASP_HPP
