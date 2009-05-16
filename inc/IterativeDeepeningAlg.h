
#ifndef ITERATIVE_DEEPENING_ALG
#define ITERATIVE_DEEPENING_ALG


#include "Node.h"
#include "SearchAlg.h"
#include "GnuplotWriter.h"

#include <vector>
#include <string>
#include <string.h>
#include <stack>

class IterativeDeepeningAlg : public SearchAlg {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
    
    public:

    private:
        int currMaxDepth;
        GnuplotWriter* gw;
        stack<Position> q;




  
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
 
    public:
        IterativeDeepeningAlg(vector<vector<Node *> > *map, GnuplotWriter *writer);
        string DoSearch(Position startNode, Position endNode);
        vector<Node *> GetBestPath();

    private:
        string EnqueueNeighbors(Position p);
        string EnQ(int row, int col, Position from);

};


#endif




















