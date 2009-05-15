
#ifndef DEPTH_FIRST_ALG
#define DEPTH_FIRST_ALG


#include "Node.h"
#include "SearchAlg.h"
#include "GnuplotWriter.h"

#include <vector>
#include <string>
#include <string.h>
#include <stack>


class DepthFirstAlg : public SearchAlg {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
    
    public:

    private:
        GnuplotWriter* gw;
        stack<Position> q;



  
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
 
    public:
        DepthFirstAlg(vector<vector<Node *> > *map, GnuplotWriter* writer);
        void DoSearch(Node *startNode);
        vector<Node *> GetBestPath();

    private:
        string EnqueueNeighbors(Position p);
        string EnQ(int row, int col, Position from);   private:
};


#endif



















