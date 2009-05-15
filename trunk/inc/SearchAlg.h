
#ifndef SEARCH_ALG
#define SEARCH_ALG


#include <iostream>
#include <vector>
#include <string.h>
#include <string>

#include "Node.h"

using namespace std;


class SearchAlg {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
    
    public:
        vector<vector<Node *> > *map;//need to either do this dynamically or hard-code the bounds in it...

    private:
   


    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */
 
    public:
        SearchAlg(vector< vector<Node *> > *map);
        virtual void DoSearch(Node *startNode) = 0;
        virtual vector<Node *> GetBestPath() = 0;

    private:

};






#endif

























