
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
       int row, col;
       Position(int r, int c) {
           row = r;
           col = c;
       }
       void set(int r, int c) {
           this->row = r;
           this->col = c;
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

























