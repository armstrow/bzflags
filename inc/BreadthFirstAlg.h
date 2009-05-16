
#ifndef BREADTH_FIRST_ALG
#define BREADTH_FIRST_ALG


#include "Node.h"
#include "SearchAlg.h"
#include "GnuplotWriter.h"

#include <vector>
#include <string>
#include <string.h>
#include <queue>

using namespace std;


class BreadthFirstAlg : public SearchAlg {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
    
    public:

    private:
        GnuplotWriter* gw;
        queue<Position> q;

  
    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */
 
    public:
        BreadthFirstAlg(vector<vector<Node *> > *map, GnuplotWriter* writer);
        string DoSearch(Position startNode, Position endNode);
        vector<Node *> GetBestPath();

    private:
        string EnqueueNeighbors(Position p);
        string EnQ(int row, int col, Position from);
};


#endif




















