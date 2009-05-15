
#ifndef ITERATIVE_DEEPENING_ALG
#define ITERATIVE_DEEPENING_ALG


#include "Node.h"
#include "SearchAlg.h"
#include "GnuplotWriter.h"

#include <vector>
#include <string>
#include <string.h>
#include <queue>

class IterativeDeepeningAlg : public SearchAlg {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
    
    public:

    private:
        GnuplotWriter* gw;
        queue<Position> q;




  
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
 
    public:
        IterativeDeepeningAlg(vector<vector<Node *> > *map, GnuplotWriter *writer);
        string DoSearch(Node startNode, Node endNode);
        vector<Node *> GetBestPath();

    private:
};


#endif




















