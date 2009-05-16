
#ifndef SEARCH_ALG
#define SEARCH_ALG


#include <iostream>
#include <vector>
#include <string.h>
#include <string>

#include "Node.h"
#include "GnuplotWriter.h"

using namespace std;


class Position{
    public:
        int row, col, level;
        float distSoFar;
        float heuristic;
        Position* from;
        Position(int r, int c) {
            row = r;
            col = c;
            level = 0;
            heuristic = 0;
            distSoFar = 0;
            from = NULL;
        }
        void set(int r, int c) {
            this->row = r;
            this->col = c;
        }
};

class ComparePosition {
    public:
        bool operator()(Position &p1, Position &p2) {
            return p1.heuristic > p2.heuristic;
        }
};


class SearchAlg {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
    
    public:
        vector<vector<Node *> > *map;

    private:
   


    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */
 
    public:
        SearchAlg(vector< vector<Node *> > *map, GnuplotWriter* writer){ }
        virtual string DoSearch(Position startNode, Position endNode) = 0;
        virtual vector<Node *> GetBestPath() = 0;

    private:

};






#endif

























