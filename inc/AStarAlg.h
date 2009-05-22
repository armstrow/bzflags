
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
        bool penalize;
        EnvironmentData *env;

    private:
        GnuplotWriter* gw;
        priority_queue<Position *, vector<Position *>, ComparePosition> q;

  
    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */
 
    public:
        AStarAlg(vector<vector<Node *> > *map, GnuplotWriter* writer, bool penalized, EnvironmentData *env);
        string DoSearch(Position startNode, Position endNode, vector<Position> *finalPath);
        string DoSearch(Position startNode, Position endNode);
        vector<Node *> GetBestPath();

    private:
        void ResetMap();
        float GetRealDistance(Position start, Position end);
        void GetCenter(float *centerX, float *centerY, vector<Point> points);
        Position GetNode(float xloc, float yloc);
        string EnqueueNeighbors(Position* p, Position endNode);
        string EnQ(int row, int col, Position* from, Position endNode);
        float GetHeuristic(int row, int col, Position pos);
};


#endif




















