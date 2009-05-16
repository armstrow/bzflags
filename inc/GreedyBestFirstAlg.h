
#ifndef GREEDY_BEST_FIRST_ALG
#define GREEDY_BEST_FIRST_ALG


#include "Node.h"
#include "SearchAlg.h"
#include "GnuplotWriter.h"
#include "EnvironmentData.h"

#include <vector>
#include <string>
#include <string.h>
#include <queue>

using namespace std;


class GreedyBestFirstAlg : public SearchAlg {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
    
    public:
        bool penalize;
        EnvironmentData *env;

    private:
        GnuplotWriter* gw;
        priority_queue<Position, vector<Position>, ComparePosition> q;

  
    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */
 
    public:
        GreedyBestFirstAlg(vector<vector<Node *> > *map, GnuplotWriter* writer, bool penalized, EnvironmentData *env);
        string DoSearch(Position startNode, Position endNode);
        vector<Node *> GetBestPath();

    private:
        void GetCenter(float *centerX, float *centerY, vector<Point> points);
        void GetCenter(vector<Point> points);
        Position GetNode(float xloc, float yloc);
        string EnqueueNeighbors(Position p, Position endNode);
        string EnQ(int row, int col, Position from, Position endNode);
        float GetHeuristic(int row, int col, Position pos);
};


#endif




















