#ifndef GRASP_HPP
#define GRASP_HPP
#include <lemon/list_graph.h>
using namespace lemon;

class Grasp {
    public:
        Grasp();
        void execute(ListGraph *g, lemon::ListGraph::NodeMap<bool>* terminal, lemon::ListGraph::EdgeMap<double>* length);
};

#endif  //GRASP_HPP
