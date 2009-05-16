
#ifndef A_STAR_ALG
#define A_STAR_ALG


#include "Node.h"
#include "SearchAlg.h"
#include "GnuplotWriter.h"

#include <vector>
#include <string>
#include <string.h>
#include <queue>

using namespace std;


class AStarAlg : public SearchAlg {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
    
    public:

    private:
        GnuplotWriter* gw;
        priority_queue<Position, vector<Position>, ComparePosition> q;

  
    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */
 
    public:
        AStarAlg(vector<vector<Node *> > *map, GnuplotWriter* writer);
        string DoSearch(Position startNode, Position endNode);
        vector<Node *> GetBestPath();

    private:
        string EnqueueNeighbors(Position p, Position endNode);
        string EnQ(int row, int col, Position from, Position endNode);
        float GetHeuristic(int row, int col, Position pos);
};


#endif




















